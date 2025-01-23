#pragma once
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/TextureManager.h"
#include <array>

class Player;

class Stars {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

	// setter
	void SetPlayer(Player* player) { player_ = player; }

private:
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// パーティクルの個数
	static inline const uint32_t numParticles_ = 24;

	std::array<KamataEngine::WorldTransform, numParticles_> worldTransforms_;

	// 星の移動量、回転量を決める(初期化でのみ行う)
	float moveTranslationZ_ = 0.0f;
	KamataEngine::Vector3 moveRotate_{0.0f, 0.0f, 0.0f};

	Player* player_ = nullptr;


};
