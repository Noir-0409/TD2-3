#include "Stars.h"
#include "Player.h"
#include <cassert>
#include <random>
#include "kMath.h"
#include <math.h>
#define _USE_MATH_DEFINES

using namespace KamataEngine;

void Stars::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	// 乱数生成の初期化
	std::random_device seedGenarator;
	std::mt19937 randomEngine(seedGenarator());
	// 乱数範囲
	std::uniform_real_distribution<float> distributionXY{-200.0f, 200.0f};
	std::uniform_real_distribution<float> distributionZ{200.0f, 1000.0f};
	std::uniform_real_distribution<float> distributionRotate{0.0f, float(M_PI) / 2};
	std::uniform_real_distribution<float> distributionScale{0.5f, 1.5f};

	moveRotate_ = {distributionRotate(randomEngine) / 20.0f, distributionRotate(randomEngine) / 100.0f, distributionRotate(randomEngine) / 200.0f};

	moveTranslationZ_ = distributionScale(randomEngine);

	for (auto& worldTransform : worldTransforms_) {

		worldTransform.Initialize();
		worldTransform.translation_ = {distributionXY(randomEngine), distributionXY(randomEngine), distributionZ(randomEngine)};
		worldTransform.rotation_.y = distributionRotate(randomEngine);
		worldTransform.scale_ = {distributionScale(randomEngine), distributionScale(randomEngine), distributionScale(randomEngine)};
	}
}

void Stars::Update() {
	for (auto& worldTransform : worldTransforms_) {

		Vector3 playerPos = player_->GetWorldPosition();
		if (worldTransform.translation_.z < playerPos.z - 2.0f) {
			// 乱数生成の初期化
			std::random_device seedGenarator;
			std::mt19937 randomEngine(seedGenarator());
			// 乱数範囲
			std::uniform_real_distribution<float> distributionXY{-200.0f, 200.0f};
			std::uniform_real_distribution<float> distributionZ{500.0f, 1000.0f};
			std::uniform_real_distribution<float> distributionRotate{0.0f, float(M_PI) / 2};
			std::uniform_real_distribution<float> distributionScale{0.5f, 1.5f};


			worldTransform.translation_ = {distributionXY(randomEngine), distributionXY(randomEngine), distributionZ(randomEngine)};
			worldTransform.rotation_.y = distributionRotate(randomEngine);
			worldTransform.scale_ = {distributionScale(randomEngine), distributionScale(randomEngine), distributionScale(randomEngine)};
		}

		worldTransform.translation_.z -= moveTranslationZ_;
		worldTransform.rotation_ += moveRotate_;

		worldTransform.UpdateMatrix();
	}
}

void Stars::Draw(Camera& camera) {
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, camera);
	}
}
