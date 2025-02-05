#include "GameScene.h"
#include <cassert>
#include <3d\AxisIndicator.h>
#include "fstream"
#include <imgui.h>
#include <algorithm>


using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete fade_;
	delete player_;
	delete stars_;
	delete planets_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	for (EnemyTrackingBullet* enemyTrackingBullet : enemyTrackingBullets_) {
		delete enemyTrackingBullet;
	}
	delete debugCamera_;
	delete fogSprite_;
	delete dedSprite_;
}

void GameScene::Initialize() {

	SetCursorPos(960, 540);
	mousePos_ = GetMousePosition();
	mouseSensi_ = {0.5f, 1.7f};

	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	debugCamera_ = new DebugCamera(1920, 1080);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するカメラを指定する(アドレス無し)
	AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	fade_ = new Fade();
	fade_->Initialize();

	// プレイヤーの初期化
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("player");
	modelPlayerBullet_ = KamataEngine::Model::CreateFromOBJ("cube");
	player_ = new Player();
	player_->Initialize(modelPlayer_, modelPlayerBullet_, Vector3{ 0.0f, 0.0f, 0.0f });
	player_->SetGameScene(this);
	useTarget_ = player_->UseTarget();

	// レールカメラ
	//railCamera_ = new RailCamera();
	//railCamera_->Initialize(player_->GetWorldTransform());
	//// 自キャラとレールカメラの親子関係を結ぶ
	////player_->SetParent(&railCamera_->GetWorldTransform());
	//railCamera_->SetParent(&player_->GetWorldTransform());

	// 敵の初期化
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube");
	LoadEnemyPopData();

	// 天球の初期化
	modelSkydome_ = Model::CreateFromOBJ("skyDome");
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkydome_);

	//霧の初期化
	fogTextureHandle_ = KamataEngine::TextureManager::Load("fog.png");
	fogSprite_ = Sprite::Create(fogTextureHandle_, { 0.0f,0.0f });

	//瀕死エフェクトの初期化
	dedTextureHandle_ = KamataEngine::TextureManager::Load("dedEffect.png");
	dedSprite_ = Sprite::Create(dedTextureHandle_, { 0.0f,0.0f });

	modelStars_ = Model::CreateFromOBJ("star");
	stars_ = new Stars();
	stars_->Initialize(modelStars_);
	stars_->SetPlayer(player_);

	modelPlanets_ = Model::CreateFromOBJ("planet");
	planets_ = new Planets();
	planets_->Initialize(modelPlanets_, planet_);

	player_->SetPlanets(planets_);

	worldTransform_.Initialize();
	planetWorldTransform_.Initialize();
	camera_.farZ = 2000.0f;
	camera_.Initialize();

	previousTime_ = std::chrono::steady_clock::now();

	isCleard_ = false;
	finished_ = false;

	// Phase移行時のバグ回避のため一度だけInitializeでUpdate
	UpdateEnemyPopCommands();
	player_->Update();
	// railCamera_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
	// デスフラグの立った弾を削除
	enemyTrackingBullets_.remove_if([](EnemyTrackingBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyTrackingBullet* enemyTrackingBullet : enemyTrackingBullets_) {
		enemyTrackingBullet->Update();
	}
	skyDome_->Update();
	stars_->Update();
	planets_->Update();
	CheckAllCollisions();
	if (player_->UseTarget()) {
		CheckLockOn();
	}
	planetWorldTransform_.UpdateMatrix();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::Update() {
	UpdateCursor();
#ifdef _DEBUG
	player_->UpdateImgui();
	planets_->UpdateImgui();
	// railCamera_->UpdateImgui();
	if (input_->TriggerKey(DIK_AT)) {
		printf("");
	}
	ImGui::Begin("mouseSensi");
	ImGui::DragFloat2("sensi", &mouseSensi_.x, 0.1f);
	ImGui::DragFloat("fov", &camera_.fovAngleY, 0.01f);
	ImGui::End();
#endif // _DEBUG

	// メニューを開いていているかどうか
	// 現在の時間を取得
	//auto currentTime = std::chrono::steady_clock::now();

	//// 前回の時間との差を計算（経過時間）
	//std::chrono::duration<float> deltaTime = currentTime - previousTime_;

	//// deltaTime（経過時間）を次回のフレームに使うために記録
	//previousTime_ = currentTime;

	//ChangeFogAlpha(deltaTime.count());
	//ChangeDedAlpha(deltaTime.count());

	switch (phase_) {
	case FadePhase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kMain;
			break;
		case FadePhase::kMain:
			if (!showMenu_) {
				UpdateEnemyPopCommands();
				player_->Update();
				if (player_->IsDead()) {
					// マウスカーソルの表示
					showCursor_ = !showCursor_;
					cursor = ShowCursor(showCursor_);
					SetCursorPos(960, 540);
					if (cursor >= 0) {
						cursor = 1;
					} else if (cursor <= 0) {
						cursor = -1;
					}
					fade_->Start(Fade::Status::FadeOut, 1.0f); // フェード
					phase_ = FadePhase::kFadeOut;
				}
				// railCamera_->Update();
				for (Enemy* enemy : enemies_) {
					enemy->Update();
				}
				for (Enemy* enemy : enemies_) {
					if (enemy->IsDead()) {
						player_->SetTarget(false);
					}
				}
				enemies_.remove_if([](Enemy* enemy) {
					if (enemy->IsDead()) {
						delete enemy;
						return true;
					}
					return false;
				});
				// デスフラグの立った弾を削除
				enemyBullets_.remove_if([](EnemyBullet* bullet) {
					if (bullet->IsDead()) {
						delete bullet;
						return true;
					}
					return false;
				});

				for (EnemyBullet* enemyBullet : enemyBullets_) {
					enemyBullet->Update();
				}
				// デスフラグの立った弾を削除
				enemyTrackingBullets_.remove_if([](EnemyTrackingBullet* bullet) {
					if (bullet->IsDead()) {
						delete bullet;
						return true;
					}
					return false;
				});
				for (EnemyTrackingBullet* enemyTrackingBullet : enemyTrackingBullets_) {
					enemyTrackingBullet->Update();
				}
				skyDome_->Update();
				stars_->Update();
				planets_->Update();
				CheckAllCollisions();
				if (player_->UseTarget()) {
					CheckLockOn();
				}

				switch (planets_->GetPlanet()) {
				case Planet::normal:

					if (planet_ != Planet::normal) {
						planet_ = Planet::normal;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

					/*if (input_->TriggerKey(DIK_RETURN)) {
						planet_ = Planet::control;
					}*/

					break;

				case Planet::control:

					// 操作を反転
					player_->InvertControls();

					if (planet_ != Planet::control) {
						planet_ = Planet::control;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

					/*if (input_->TriggerKey(DIK_RETURN)) {

						planet_ = Planet::fog;
					}*/

					break;

				case Planet::fog:

					if (planet_ != Planet::fog) {
						planet_ = Planet::fog;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

					// 視界悪化

					/*if (input_->TriggerKey(DIK_RETURN)) {

						planet_ = Planet::heal;
					}*/

					break;

				case Planet::obstacle:

					// 障害物

					if (planet_ != Planet::obstacle) {
						planet_ = Planet::obstacle;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

				/*	if (input_->TriggerKey(DIK_RETURN)) {
						planet_ = Planet::time;
					}*/

					break;

				case Planet::time:

					// 時間の流れ
					player_->TimeFlow();

					if (planet_ != Planet::time) {
						planet_ = Planet::time;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

				/*	if (input_->TriggerKey(DIK_RETURN)) {

						planet_ = Planet::gravity;
					}*/

					break;

				case Planet::gravity:

					// 重力
					player_->AffectGravity();

					if (planet_ != Planet::gravity) {
						planet_ = Planet::gravity;
						enemyPopCommands.str(""); // ストリームの内容をクリア
						enemyPopCommands.clear(); // ストリームのエラー状態をリセット
						OnPlanetChange();
					}

				/*	if (input_->TriggerKey(DIK_RETURN)) {

						planet_ = Planet::normal;
					}*/

					break;
				}

				planetWorldTransform_.UpdateMatrix();
			} else {
				if (input_->TriggerKey(DIK_T)) {
					useTarget_ = !useTarget_;
					player_->SetUseTarget(useTarget_);
				}
			}
			break;
		case FadePhase::kFadeOut:
			if (fade_->IsFinished()) {
				if (!player_->IsDead()) {
					isCleard_ = true;
				} else {
					finished_ = true;
				}
				phase_ = FadePhase::kFadeIn;
			}
			break;
		}
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// カメラ行列の転送
		camera_.TransferMatrix();
	} else {
		// カメラ行列の更新と転送
		camera_.UpdateMatrix();
	}
	// camera_.matView = railCamera_->GetCamera().matView;
	camera_.matView = player_->GetCamera().matView;
	// camera_.matProjection = railCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();
	// worldTransform_.UpdateMatirx();

	// 現在の時間を取得
	auto currentTime = std::chrono::steady_clock::now();

	// 前回の時間との差を計算（経過時間）
	std::chrono::duration<float> deltaTime = currentTime - previousTime_;

	// deltaTime（経過時間）を次回のフレームに使うために記録
	previousTime_ = currentTime;

	ChangeFogAlpha(deltaTime.count());
	ChangeDedAlpha(deltaTime.count());


	// 最終的なクリアまでの時間決めたら戻す!!
	 clearTimer_--;

	if (clearTimer_ == 0) {

		isCleard_ = true;
	}

	//if (player_->IsDead()) {

		//finished_ = true;
	//}*/

	fade_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skyDome_->Draw(worldTransform_, camera_);
	stars_->Draw(camera_);
	planets_->Draw(camera_);
	player_->Draw(camera_);
	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(camera_);
	}
	for (EnemyTrackingBullet* enemyTrackingBullet : enemyTrackingBullets_) {
		enemyTrackingBullet->Draw(camera_);
	}

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	fogSprite_->Draw();
	dedSprite_->Draw();

	fade_->Draw(commandList);

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*> enemyBullets = GetEnemyBullets();
	const std::list<EnemyTrackingBullet*> enemyTrackingBullets = GetEnemyTrackingBullets();

#pragma	 region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
		float len = pow((1.0f + 1.0f), 2.0f);

		// 弾と弾の交差判定
		if (dist <= len && !player_->IsDamage()) {
			// 自キャラの衝突時コールバックを呼び出す
			if (!player_->IsDamage()) {
				player_->OnCollision();
			}
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	// 自キャラと敵弾全ての当たり判定
	for (EnemyTrackingBullet* bullet : enemyTrackingBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
		float len = pow((1.0f + 1.0f), 2.0f);

		// 弾と弾の交差判定
		if (dist <= len) {
			// 自キャラの衝突時コールバックを呼び出す
			if (!player_->IsDamage()) {
				player_->OnCollision();
			}
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
			float len = pow((1.0f + 1.0f), 2.0f);

			// 弾と弾の交差
			if (dist <= len && !enemy->IsDamage()) {
				// 敵キャラの衝突コールバックを呼び出す
				if (!enemy->IsDamage()) {
					enemy->OnCollision();
				}
				// 自弾の衝突コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
			float len = pow((1.0f + 1.0f), 2.0f);

			// 弾と弾の交差
			if (dist <= len) {
				// 自弾の衝突コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyTrackingBullet* enemyTrackingBullet : enemyTrackingBullets) {
			// 敵弾の座標
			posB = enemyTrackingBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
			float len = pow((1.0f + 1.0f), 2.0f);

			// 弾と弾の交差
			if (dist <= len) {
				// 自弾の衝突コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突コールバックを呼び出す
				enemyTrackingBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::CheckLockOn() { 
	Vector3 posA, posB, posC;

	posA = player_->GetWorldPosition();
	posB = player_->GetTargetWorldPosition();

	Vector3 TargetVector = posB - posA;
	TargetVector = Normalize(TargetVector);
	
	for (Enemy* enemy : enemies_) {
		posC = enemy->GetWorldPosition();
		Vector3 enemyVector = posC - posA;
		enemyVector = Normalize(enemyVector);
		float rotate = std::acos(Dot(TargetVector, enemyVector));

		if (rotate <= 0.1f && !player_->IsTarget()) {
			lockonTimer_ += 1.0f / 60 / lockonTime_;
		} else if (rotate > 0.1f&& enemy->IsTarget()) {
			enemy->SetTarget(false);
			player_->SetTarget(false);
		}
		if (lockonTimer_ >= 1.0f) {
			lockonTimer_ = 0.0f;
			enemy->SetTarget(true);
			player_->SetTarget(true);
			player_->SetTargetPositoin(posC);
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemyTrackingBullet(EnemyTrackingBullet* enemyTrackingBullet) {
	// リストに登録する
	enemyTrackingBullets_.push_back(enemyTrackingBullet);
}

void GameScene::UpdateCursor() {
	// ゲーム中
	if (!showCursor_) {
		Vector2 mousePos = GetMousePosition();
		if (mousePos.x != 960.0f) {
			mousePos_.x += (mousePos.x - 960.0f) * mouseSensi_.x;
			SetCursorPos(960, 540);
		}
		if (mousePos.y != 540.0f) {
			mousePos_.y += (mousePos.y - 540.0f) * mouseSensi_.y;
			SetCursorPos(960, 540);
		}
	}
	mousePos_.x = std::clamp(mousePos_.x, 0.0f, 1920.0f);
	mousePos_.y = std::clamp(mousePos_.y, 0.0f, 1000.0f);

	// メニュー
	if (input_->TriggerKey(DIK_ESCAPE) && !player_->IsDead()) {
		showCursor_ = !showCursor_;
		showMenu_ = showCursor_;
		cursor = ShowCursor(showCursor_);
		SetCursorPos(960, 540);
	}
	if (cursor >= 0) {
		cursor = 1;
	} else if (cursor <= 0) {
		cursor = -1;
	}
}

Vector2 GameScene::GetMousePosition() {
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	Vector2 mousePosition;
	mousePosition.x = float(mousePoint.x);
	mousePosition.y = float(mousePoint.y);

	return mousePosition;
}

void GameScene::ChangeFogAlpha(float deltaTime) {

	if (planet_ == Planet::fog) {
		//霧の濃さを増加させる
		fogAlpha_ += fogAlphaStep_ * deltaTime;

		//アルファ値の制限
		if (fogAlpha_ > 0.9f) fogAlpha_ = 0.9f;
	} else {
		//他のフェーズでは霧を薄くしていく
		fogAlpha_ -= fogAlphaStep_ * deltaTime;

		//アルファ値の制限
		if (fogAlpha_ < 0.0f) fogAlpha_ = 0.0f;
	}

	//スプライトに反映
	fogSprite_->SetColor({ 1.0f, 1.0f, 1.0f, fogAlpha_ });
}

void GameScene::ChangeDedAlpha(float deltaTime)
{
	
	if (player_->IsLowHP() && !player_->IsDead()) {
		
		if (dedAlpha_ >= 0.9f) {
			dedAlphaStep_ = -abs(dedAlphaStep_); 
		}
		
		else if (dedAlpha_ <= 0.0f) {
			dedAlphaStep_ = abs(dedAlphaStep_); 
		}

	
		dedAlpha_ += dedAlphaStep_ * deltaTime;
	}
	
	else {
		

		dedAlpha_ -= abs(dedAlphaStep_) * deltaTime;
		if (dedAlpha_ < 0.0f) dedAlpha_ = 0.0f;

	}

	// スプライトに反映
	dedSprite_->SetColor({ 1.0f, 1.0f, 1.0f, dedAlpha_ });
}

void GameScene::RespawnEnemies()
{

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	// ここで敵を再発生
	spawnTimer = 0;

}

void GameScene::OnPlanetChange()
{

	//RespawnEnemies();
	LoadEnemyPopData();
	UpdateEnemyPopCommands();

}


// 敵発生コマンド
void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

// 敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			waitFlag = false;
		}
		return;
	}

	if (spawnTimer > 0) {
		spawnTimer--;
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {

			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::stof(word.c_str());

			// 弾の種類
			/*bool normalBullet = true;
			std::getline(line_stream, word, ',');
			if (word.find("tracking") == 0) {
				normalBullet = false;
			} else {
				normalBullet = true;
			}*/

			// 敵を発生させる
			Enemy* enemy = new Enemy();
			enemy->SetPlayer(player_);
			if (planets_->GetPlanet() == Planet::newEnemy) {
				enemy->Initialize(modelEnemy_, Vector3{x, y, z}, Vector3{0.0f, 0.0f, 0.0f}, 100.0f, BulletType::tracking);
			} else {
				enemy->Initialize(modelEnemy_, Vector3{x, y, z}, Vector3{0.0f, 0.0f, 0.0f}, 100.0f, BulletType::normal);
			}
			enemy->SetGameScene(this);
			enemy->SetPlanets(planets_);
			enemies_.push_back(enemy);

		}
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());
			// 待機開始
			waitFlag = true;
			waitTimer = waitTime;

			// ループを抜ける
			break;
		}
	}
}