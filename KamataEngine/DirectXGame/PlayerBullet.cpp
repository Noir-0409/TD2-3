#include "PlayerBullet.h"

using namespace KamataEngine;

void PlayerBullet::initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;

	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void PlayerBullet::update()
{

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	//時間経過で消滅
	if (--deathTimer_ <= 0) {

		isDead_ = true;

	}

	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const KamataEngine::Camera& camera)
{

	model_->Draw(worldTransform_, camera, textureHandle_);

}

KamataEngine::Vector3 PlayerBullet::GetWorldPosition()
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

void PlayerBullet::OnCollision()
{

	isDead_ = true;

}
