#include "Player.h"
#include <imgui.h>
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

void Player::Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	input_ = KamataEngine::Input::GetInstance();
	assert(model);
	model_ = model;

	targetModel_ = Model::CreateFromOBJ("target");
	worldTransform_.Initialize();
	targetWorldTransform_.Initialize();
	targetWorldTransform_.translation_ = position;
	worldTransform_.translation_ = position;
	objectColor_.Initialize();
	targetObjectColor_.Initialize();
	targetObjectColor_.SetColor(Vector4{65.0f, 255.0f, 75.0f, 1.0f});
	camera_ = camera;
}

void Player::Update() {
	mousePos_ = gameScene_->GetMousePos();
	wolk();
	TargetUpdate();
	Rotate();
	Attack();

	// ダメージを受けたら実行
	if (isDamage_) {
		damageDelayTimer_ += 1.0f / 60 / damageDelayTime_ * 10.0f;
		if (damageDelayTimer_ >= 10.0f) {
			isDamage_ = false;
			damageDelayTimer_ = 0.0f;
			inDamageDrawCounter_ = 0;
		}
		inDamageDrawCounter_ += 1;
	}

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
	ImGui::End();
#endif // _DEBUG
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
	mousePos_.x = std::clamp(mousePos_.x, 0.0f, 1980.0f);
	mousePos_.y = std::clamp(mousePos_.y, 0.0f, 1000.0f);
	// 照準の回転を変更して動かす
	targetWorldTransform_.rotation_.y = (mousePos_.x - 990.0f) / 1920;
	targetWorldTransform_.rotation_.x = (mousePos_.y - 540.0f) / 2040;
	targetWorldTransform_.rotation_.y = std::clamp(targetWorldTransform_.rotation_.y, (0 - 990.0f) / 1920, (1919 - 990.0f) / 1920);
	targetWorldTransform_.rotation_.x = std::clamp(targetWorldTransform_.rotation_.x, (0 - 990.0f) / 2040, (1919 - 990.0f) / 2040);
	targetWorldTransform_.scale_ = worldTransform_.scale_;
}

void Player::Attack() {
	if (useTarget_ && isTarget_) {
		if (input_->PushKey(DIK_SPACE) && !isAttack_) {

			isAttack_ = true;
			// 弾の速度
			const float kBulletSpeed = 5.0f;
			Vector3 worldPos = GetWorldPosition();
			Vector3 velocity = targetWorldPosition_ - worldPos;
			velocity = Normalize(velocity);
			velocity *= kBulletSpeed;
			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, Vector3{GetWorldPosition().x, GetWorldPosition().y - 1, GetWorldPosition().z}, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		} else if (isAttack_) { // 攻撃間隔
			fireDelayTimer_ += 1.0f / 60 / fireDelayTime_;
			if (fireDelayTimer_ >= 1.0f) {
				isAttack_ = false;
				fireDelayTimer_ = 0.0f;
				/*fireDelayTime_ -= 0.08f;
				fireDelayTime_ = std::clamp(fireDelayTime_, 0.05f, 0.5f);*/
			}
		}
	} else {
		if (input_->PushKey(DIK_SPACE) && !isAttack_) {
			isAttack_ = true;
			// 弾の速度
			const float kBulletSpeed = 5.0f;
			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);
			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, targetWorldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, Vector3{GetWorldPosition().x, GetWorldPosition().y - 1, GetWorldPosition().z}, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		} else if (isAttack_) { // 攻撃間隔
			fireDelayTimer_ += 1.0f / 60 / fireDelayTime_;
			if (fireDelayTimer_ >= 1.0f) {
				isAttack_ = false;
				fireDelayTimer_ = 0.0f;
				/*fireDelayTime_ -= 0.08f;
				fireDelayTime_ = std::clamp(fireDelayTime_, 0.05f, 0.5f);*/
			}
		}
	}
	/*if (!input_->PushKey(DIK_SPACE)) {
		fireDelayTime_ = 0.5f;
	}*/
}


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
	hp_ -= 0;
	isDamage_ = true;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親関係を結ぶ
	worldTransform_.parent_ = parent;
}