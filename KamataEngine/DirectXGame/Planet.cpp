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