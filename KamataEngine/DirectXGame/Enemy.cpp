#include "Enemy.h"
#include <cassert>
#include "MathUtility.h"
#include "player.h"

using namespace KamataEngine;

Enemy::~Enemy()
{

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Enemy::Initialze(KamataEngine::Model* model, uint32_t textureHandle, const KamataEngine::Vector3& position)
{

	//NULLチェック
	assert(model);

	//引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	ApproachInitialize();

}

void Enemy::Update()
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {

		if (bullet->IsDead()) {

			delete bullet;
			return true;

		}

		return false;

		}

	);

	switch (phase_) {

	case Phase::Approach:

		Approach();

		break;

	case Phase::Leave:


		Leave();

		break;

	}

	//Fire();

	//弾更新
	for (EnemyBullet* bullet : bullets_) {

		bullet->Update();

	}

	//行列更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(KamataEngine::Camera* camera)
{

	//描画
	model_->Draw(worldTransform_, *camera, textureHandle_);

	//弾更新
	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(*camera);

	}

}

void Enemy::Approach()
{

	//発射タイマーをカウントダウン
	fireTimer--;

	//指定時間に達した
	if (fireTimer <= 0) {

		//弾を発射
		Fire();

		//発射タイマーを初期化
		fireTimer = kFireInterval;

	}

	//キャラクターの移動ベクトル
	KamataEngine::Vector3 move = { 0,0,0 };

	//キャラクターの移動速度
	const float kEnemySpeed = 0.1f;

	move.z -= kEnemySpeed;

	if (worldTransform_.translation_.z < 0.0f) {

		phase_ = Phase::Leave;

	}

	//座標移動
	worldTransform_.translation_ += move;

}

void Enemy::ApproachInitialize()
{

	//発射タイマーを初期化
	fireTimer = kFireInterval;

}

void Enemy::Leave()
{

	//キャラクターの移動ベクトル
	KamataEngine::Vector3 move = { 0,0,0 };

	//キャラクターの移動速度
	const float kEnemySpeed = 0.1f;

	move.x -= kEnemySpeed;
	//	move.y += kEnemySpeed;

		//座標移動
	worldTransform_.translation_ += move;

}

void Enemy::Fire()
{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 0.7f;

	// 自キャラのワールド座標を取得
	KamataEngine::Vector3 playerPos = player_->GetWorldPosition();

	// 敵のワールド座標を取得
	KamataEngine::Vector3 enemyPos = GetWorldPosition();

	// 敵から自キャラへの差分ベクトルを求める
	KamataEngine::Vector3 direction = playerPos - enemyPos;

	// ベクトルの正規化
	direction = KamataEngine::MathUtility::Normalize(direction);

	// ベクトルの長さを速さに合わせる
	KamataEngine::Vector3 velocity;
	velocity.x = direction.x * kBulletSpeed * -1;
	velocity.y = direction.y * kBulletSpeed * -1;
	velocity.z = direction.z * kBulletSpeed * -1;

	// 弾を生成して初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, enemyPos, velocity);

	// 弾を登録
	bullets_.push_back(newBullet);
}


KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	// ワールド行列のTx
	worldPos.x = worldTransform_.translation_.x;

	// ワールド行列のTy
	worldPos.y = worldTransform_.translation_.y;

	// ワールド行列のTz
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}
