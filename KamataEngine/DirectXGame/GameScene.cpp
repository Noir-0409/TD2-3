#include "GameScene.h"
#include <cassert>
#include "3d/AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;

	delete player_;

	delete enemy_;

	delete debugCamera_;

	delete skydome_;

	delete modelSkydome_;

}

void GameScene::Initialze() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	enemyTextureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	//3Dモデルの生成
	model_ = Model::Create();

	//天球モデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//ビュープロジェクション(カメラ)の初期化
	camera_.Initialize();

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の生成
	enemy_ = new Enemy();

	//敵の初期化
	enemy_->Initialze(model_, enemyTextureHandle_, { 10.0f,10.0f,50.0f });

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_ = new Skydome();

	//天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

}

void GameScene::Update() {

	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

	//天球の更新
	skydome_->Update();

	//デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {

		isDebugcameraActive_ = true;

	}

#endif // _DEBUG


	//カメラの処理
	if (isDebugcameraActive_) {

		debugCamera_->Update();

		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;

		//ビュープロジェクション行列の転送
		camera_.TransferMatrix();

	} else {

		//ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();

	}

	CheckAllCollision();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//自キャラの描画
	player_->Draw(&camera_);

	//敵の描画
	enemy_->Draw(&camera_);

	//天球の描画
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::CheckAllCollision()
{

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {

		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = std::sqrt(
			(posA.x - posB.x) * (posA.x - posB.x) +
			(posA.y - posB.y) * (posA.y - posB.y) +
			(posA.z - posB.z) * (posA.z - posB.z)
		);

		// 衝突距離の閾値（必要に応じて調整）
		const float kCollisionDistance = 1.0f;
		//弾と弾の当たり判定
		//弾同士が当たっていれば
		if (distance < kCollisionDistance) {

			player_->OnCollision();

			bullet->OnCollision();

		}

	}

#pragma endregion

#pragma region 自弾と敵の当たり判定

	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = std::sqrt(
			(posA.x - posB.x) * (posA.x - posB.x) +
			(posA.y - posB.y) * (posA.y - posB.y) +
			(posA.z - posB.z) * (posA.z - posB.z)
		);

		// 衝突距離の閾値（必要に応じて調整）
		const float kCollisionDistance = 1.0f;

		if (distance < kCollisionDistance) {

			enemy_->OnCollision();

			bullet->OnCollision();

		}

	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {

			// 自弾と敵弾の座標を取得
			posA = playerBullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を計算
			float distance = std::sqrt(
				(posA.x - posB.x) * (posA.x - posB.x) +
				(posA.y - posB.y) * (posA.y - posB.y) +
				(posA.z - posB.z) * (posA.z - posB.z)
			);


			// 衝突距離の閾値（必要に応じて調整）
			const float kCollisionDistance = 1.0f;

			// 衝突判定
			if (distance < kCollisionDistance) {
				// お互いに衝突処理を呼び出す
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion

}
