#include "Player.h"
#include <imgui.h>
#include "algorithm"
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

void Player::Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	input_ = KamataEngine::Input::GetInstance();
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	objectColor_.Initialize();
	camera_ = camera;
}

void Player::Update() {
	wolk();
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

	mousePos_ = GetMousePosition();
	//worldTransform_.rotation_.y = (mousePos_.x - 990.0f) / 200;
	//worldTransform_.rotation_.x = (mousePos_.y - 540.0f) / 200;
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

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE) && !isAttack_) {
		isAttack_ = true;
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

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
	/*if (!input_->PushKey(DIK_SPACE)) {
		fireDelayTime_ = 0.5f;
	}*/
}


void Player::Draw(KamataEngine::Camera& camera) {
	// ダメージを受けたら点滅させる
	if (isDamage_) {
		if (inDamageDrawCount_ & inDamageDrawCounter_) {
			model_->Draw(worldTransform_, camera, &objectColor_);
		}
	} else {
		model_->Draw(worldTransform_, camera, &objectColor_);
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

Vector2 Player::GetMousePosition() { 
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	Vector2 mousePosition;
	mousePosition.x = float(mousePoint.x);
	mousePosition.y = float(mousePoint.y);

	return mousePosition;
}