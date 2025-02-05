#include "Player.h"
#include <imgui.h>
#include <random>
#include "algorithm"
#include <cassert>
#include "GameScene.h"
using namespace KamataEngine;

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド座標の平行同成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// ターゲットのワールド座標を取得
Vector3 Player::GetTargetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	float dist = 10.0f;
	// playerのワールド座標を入れながらターゲットの位置を出力させる
	Vector3 world = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldPos.z = worldTransform_.matWorld_.m[3][2] + dist};
	worldPos = TransformNormal(world, targetWorldTransform_.matWorld_);
	//// ワールド座標の平行同成分を取得
	//worldPos.x = world.matWorld_.m[3][0];
	//worldPos.y = world.matWorld_.m[3][1];
	//worldPos.z = world.matWorld_.m[3][2];

	return worldPos;
}

void Player::Initialize(Model* model, Model* bulletModel, const Vector3& position) {
	input_ = KamataEngine::Input::GetInstance();
	assert(model);
	assert(bulletModel);
	model_ = model;
	bulletModel_ = bulletModel;

	targetModel_ = Model::CreateFromOBJ("target");
	//targetModel_ = Model::CreateFromOBJ("debugTarget");
	worldTransform_.Initialize();
	targetWorldTransform_.Initialize();
	camera_.Initialize();
	targetWorldTransform_.translation_ = position;
	worldTransform_.translation_ = position;
	objectColor_.Initialize();
	targetObjectColor_.Initialize();
	targetObjectColor_.SetColor(Vector4{65.0f, 255.0f, 75.0f, 1.0f});
	
	audio_ = KamataEngine::Audio::GetInstance();
	bulletDataHandle = audio_->LoadWave("./Resources./sound./player./attack.mp3");
	
}

void Player::Update() {
	mousePos_ = gameScene_->GetMousePos();
	wolk();
	TargetUpdate();
	//Rotate();
	Attack();



	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//mousePos_ = GetMousePosition();
	//worldTransform_.rotation_.y = (mousePos_.x - 990.0f) / 200;
	//worldTransform_.rotation_.x = (mousePos_.y - 540.0f) / 200;
	targetWorldTransform_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
	// ダメージを受けたら実行
	if (isDamage_) {
		damageDelayTimer_ += 1.0f / 60 / damageDelayTime_;
		damageDelayTimer_ = std::clamp(damageDelayTimer_, 0.0f, 1.0f);
		// 乱数生成の初期化
		std::random_device seedGenarator;
		std::mt19937 randomEngine(seedGenarator());
		// 乱数範囲
		std::uniform_real_distribution<float> distributionShake{-0.05f + damageDelayTimer_ / 20, 0.05f - damageDelayTimer_ / 20};
		// ダメージを受けた時にカメラをシェイクする
		if (damageShake_) {
			camera_.matView.m[3][0] += distributionShake(randomEngine);
			camera_.matView.m[3][1] += distributionShake(randomEngine);
		}
		if (damageDelayTimer_ >= 1.0f) {

			isDamage_ = false;
			damageDelayTimer_ = 0.0f;
			inDamageDrawCounter_ = 0;
		}
		inDamageDrawCounter_ += 1;
	}
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//worldTransform_.TransferMatrix();

}

void Player::UpdateImgui() {
#ifdef _DEBUG
	ImGui::Begin("PlayerState");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat("moveAmountZ", &moveAmountZ_, 0.01f);
	ImGui::DragFloat2("mousePos", &mousePos_.x, 0.01f);
	ImGui::Checkbox("useTarget", &useTarget_);
	ImGui::Checkbox("isTarget", &isTarget_);
	ImGui::Checkbox("shake", &damageShake_);
	ImGui::End();
#endif // _DEBUG
}

bool Player::IsLowHP()
{
	return hp_ <= 20;
}

void Player::InvertControls()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	if (input_->PushKey(DIK_A)) {
		move.x += kCharacterSpeed * 2;
	} else if (input_->PushKey(DIK_D)) {
		move.x -= kCharacterSpeed * 2;
	}

	if (input_->PushKey(DIK_S)) {
		move.y += kCharacterSpeed * 2;
	} else if (input_->PushKey(DIK_W)) {
		move.y -= kCharacterSpeed * 2;
	}

	worldTransform_.translation_ += move;
	// 範囲を超えないように処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

}

void Player::AffectGravity()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	move.y -= 0.1f;

	worldTransform_.translation_ += move;
	// 範囲を超えないように処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

}

void Player::TimeFlow()
{

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed * 10.0f;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed * 10.0f;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed * 10.0f;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed * 10.0f;
	}

	worldTransform_.translation_ += move;

	// 範囲を超えないように処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

}

void Player::HealHP() {

	//回復間隔
	static const int kHealInterval = 300;

	if (hp_ > 0) {

		// タイマーを増加
		healTimer_++;

		// 回復間隔に達したらHPを回復
		if (healTimer_ >= kHealInterval) {

			hp_ += 5; 

			if (hp_ > 100) {

				hp_ = 100;
			
			}
			
			//タイマーをリセット
			healTimer_ = 0;
		}

	} else {

		healTimer_ = 0;

	}
}

void Player::DamageHP()
{

	//ダメージ間隔
	static const int kDamageInterval_ = 300;

	if (hp_ > 0) {

		// タイマーを増加
		damageTimer_++;

		// ダメージ間隔に達したらHPを減らす
		if (damageTimer_ >= kDamageInterval_) {

			hp_ -= 5;
			isDamage_ = true;

			if (hp_ < 1) {

				hp_ = 1;

			}

			//タイマーをリセット
			damageTimer_ = 0;
		}

	} else {

		damageTimer_ = 0;

	}

}

void Player::PowerUp()
{

	for (Enemy* enemy : gameScene_->GetEnemies()) {

		enemy->TakeDamage();

	}

}


void Player::wolk() {
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_.z += moveAmountZ_;

	worldTransform_.translation_ += move;
	// 範囲を超えないように処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
}

void Player::Rotate() {
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::TargetUpdate() {
	// 代入
	targetWorldTransform_.translation_ = worldTransform_.translation_;
	//targetWorldTransform_.translation_.z = worldTransform_.translation_.z - 1.4f;
	mousePos_.x = std::clamp(mousePos_.x, 380.0f, 1600.0f);
	mousePos_.y = std::clamp(mousePos_.y, 180.0f, 850.0f);
	// 照準の回転を変更して動かす
	targetWorldTransform_.rotation_.y = (mousePos_.x - 960.0f) / 1920;
	targetWorldTransform_.rotation_.x = (mousePos_.y - 540.0f) / 2040;
	targetWorldTransform_.rotation_.y = std::clamp(targetWorldTransform_.rotation_.y, (0 - 990.0f) / 1920, (1919 - 990.0f) / 1920);
	targetWorldTransform_.rotation_.x = std::clamp(targetWorldTransform_.rotation_.x, (0 - 990.0f) / 2160, (1919 - 990.0f) / 2160);
	targetWorldTransform_.scale_ = worldTransform_.scale_;
}

void Player::Attack() {
	if (useTarget_ && isTarget_) {
		if (input_->PushKey(DIK_SPACE) && !isAttack_) {
			bulletVoiceHandle = audio_->PlayWave(bulletDataHandle, false, 0.7f); // 🔈音を鳴らす
			isAttack_ = true;

			// 弾の速度
			const float kBulletSpeed = 5.0f;
			Vector3 worldPos = GetWorldPosition();

			// ターゲットとの距離を算出してそこにたどり着くまでのフレーム数を計算
			// ターゲットの移動量を考慮してvelocityを求める

			float hitFrame = Length(targetWorldPosition_) - Length(worldPos);
			hitFrame = hitFrame * kBulletSpeed / 60;
			targetWorldPosition_ += targetVelocity_ * hitFrame;

			Vector3 velocity = targetWorldPosition_ - worldPos;
			velocity = Normalize(velocity);
			velocity *= kBulletSpeed;
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, Vector3{ worldPos.x, worldPos.y - 0.4f, worldPos.z + 1.0f }, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	} else {
		if (input_->PushKey(DIK_SPACE) && !isAttack_) {
			bulletVoiceHandle = audio_->PlayWave(bulletDataHandle, false, 0.7f); // 🔈ターゲットなしでも音を鳴らす
			isAttack_ = true;

			// 弾の速度
			const float kBulletSpeed = 5.0f;
			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);
			velocity = TransformNormal(velocity, targetWorldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, Vector3{ GetWorldPosition().x, GetWorldPosition().y - 0.4f, GetWorldPosition().z + 1.0f }, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	}

	// 攻撃間隔の処理（変更なし）
	if (isAttack_) {
		fireDelayTimer_ += 1.0f / 60 / fireDelayTime_;
		if (fireDelayTimer_ >= 1.0f) {
			isAttack_ = false;
			fireDelayTimer_ = 0.0f;
		}
	}
}


//void Player::Attack() {
//	
//	if (useTarget_ && isTarget_) {
//		if (input_->PushKey(DIK_SPACE) && !isAttack_) {
//
//			bulletVoiceHandle = audio_->PlayWave(bulletDataHandle, false, 0.7f);
//			isAttack_ = true;
//			// 弾の速度
//			const float kBulletSpeed = 5.0f;
//			Vector3 worldPos = GetWorldPosition();
//			Vector3 velocity = targetWorldPosition_ - worldPos;
//			velocity = Normalize(velocity);
//			velocity *= kBulletSpeed;
//			// 速度ベクトルを自機の向きに合わせて回転させる
//			velocity = TransformNormal(velocity, worldTransform_.matWorld_);
//
//			// 弾を生成し、初期化
//			PlayerBullet* newBullet = new PlayerBullet();
//			newBullet->Initialize(bulletModel_, Vector3{GetWorldPosition().x, GetWorldPosition().y - 0.4f, GetWorldPosition().z + 1.0f}, velocity);
//
//			// 弾を登録する
//			bullets_.push_back(newBullet);
//		} else if (isAttack_) { // 攻撃間隔
//			fireDelayTimer_ += 1.0f / 60 / fireDelayTime_;
//			if (fireDelayTimer_ >= 1.0f) {
//				isAttack_ = false;
//				fireDelayTimer_ = 0.0f;
//				/*fireDelayTime_ -= 0.08f;
//				fireDelayTime_ = std::clamp(fireDelayTime_, 0.05f, 0.5f);*/
//			}
//		}
//	} else {
//		if (input_->PushKey(DIK_SPACE) && !isAttack_) {
//			isAttack_ = true;
//			// 弾の速度
//			const float kBulletSpeed = 5.0f;
//			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);
//			// 速度ベクトルを自機の向きに合わせて回転させる
//			velocity = TransformNormal(velocity, targetWorldTransform_.matWorld_);
//
//			// 弾を生成し、初期化
//			PlayerBullet* newBullet = new PlayerBullet();
//			newBullet->Initialize(bulletModel_, Vector3{GetWorldPosition().x, GetWorldPosition().y - 0.4f, GetWorldPosition().z + 1.0f}, velocity);
//
//			// 弾を登録する
//			bullets_.push_back(newBullet);
//		} else if (isAttack_) { // 攻撃間隔
//			fireDelayTimer_ += 1.0f / 60 / fireDelayTime_;
//			if (fireDelayTimer_ >= 1.0f) {
//				isAttack_ = false;
//				fireDelayTimer_ = 0.0f;
//				/*fireDelayTime_ -= 0.08f;
//				fireDelayTime_ = std::clamp(fireDelayTime_, 0.05f, 0.5f);*/
//			}
//		}
//	}
//
//	/*if (!input_->PushKey(DIK_SPACE)) {
//		fireDelayTime_ = 0.5f;
//	}*/
//}


void Player::Draw(KamataEngine::Camera& camera) {
	// ダメージを受けたら点滅させる
	if (isDamage_) {
		if (inDamageDrawCount_ & inDamageDrawCounter_) {
			model_->Draw(worldTransform_, camera, &objectColor_);
			targetModel_->Draw(targetWorldTransform_, camera, &targetObjectColor_);
		}
	} else {
		model_->Draw(worldTransform_, camera, &objectColor_);
		targetModel_->Draw(targetWorldTransform_, camera, &targetObjectColor_);
	}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Player::OnCollision() {


	hp_ -= 20;
	isDamage_ = true;

	if (hp_ <= 0) {

		hp_ = 0;
		isDead_ = true;
	
	}

}


void Player::SetParent(const WorldTransform* parent) {
	// 親関係を結ぶ
	worldTransform_.parent_ = parent;
}