#include "EnemyTrackingBullet.h"
#include "Player.h"
#include <cassert>

using namespace KamataEngine;

void EnemyTrackingBullet::Initialize(Model* model, const Vector3& position, float bulletSpeed, Player* target) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// 色の変更
	objectColor_.Initialize();
	objectColor_.SetColor(Vector4{255.0f, 0.0f, 0.0f, 1.0f});

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransform_.UpdateMatrix();
	kBulletSpeed_ = bulletSpeed;
	player_ = target;
}

void EnemyTrackingBullet::Update() {
	Vector3 plWorldPos = player_->GetWorldPosition();
	Vector3 btWorldPos = GetWorldPosition();

	Vector3 BulletWorldPos = plWorldPos - btWorldPos;
	BulletWorldPos = Normalize(BulletWorldPos);
	Vector3 velocity = BulletWorldPos * kBulletSpeed_;
	worldTransform_.translation_ += velocity;
	// 時間経過で消滅(デス)
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void EnemyTrackingBullet::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera, &objectColor_); }

void EnemyTrackingBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyTrackingBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド座標の平行同成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}