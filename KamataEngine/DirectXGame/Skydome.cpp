#include "Skydome.h"
#include "cassert"
#include "Affin.h"

using namespace KamataEngine;

void Skydome::Initialize(Model* model, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.translation_ = position;
}

void Skydome::Update() {

	Vector3 move = { 0.0f,0.0f,0.0f };

	const float speed = 10000000.0f;

	//move.z -= speed;
	worldTransform_.translation_.z -= speed;

	worldTransform_.translation_.z -= move.z;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
//	worldTransform_.TransferMatrix();

}

void Skydome::Draw(WorldTransform& worldTransform, Camera& camera) {
	model_->Draw(worldTransform, camera);

}