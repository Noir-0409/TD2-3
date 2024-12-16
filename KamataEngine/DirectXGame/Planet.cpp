#include "Planet.h"
#include "cassert"

using namespace KamataEngine;

void Planet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.translation_ = position;
}

void Planet::Update()
{
	worldTransform_.UpdateMatrixEX();
}

void Planet::Draw(KamataEngine::WorldTransform& worldTransform, KamataEngine::Camera& camera)
{
	model_->Draw(worldTransform, camera);
}

void Planet::SetPosition(const KamataEngine::Vector3& position)
{
	// 新しい座標を設定
	worldTransform_.translation_ = position;

	// ワールド行列を再計算
	worldTransform_.UpdateMatrixEX();
}

KamataEngine::Vector3 Planet::GetPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド座標の平行同成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
