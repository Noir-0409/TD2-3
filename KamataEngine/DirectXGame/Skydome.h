#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "3d/Model.h"
#include "base/TextureManager.h"

class Skydome {
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
	void Draw(KamataEngine::WorldTransform& worldTransform, KamataEngine::Camera& camera);

private:
	// モデル
	KamataEngine::Model* model_ = nullptr;
};
