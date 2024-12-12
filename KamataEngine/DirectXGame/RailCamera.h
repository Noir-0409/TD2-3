#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "math/MathUtility.h"
#include "kMath.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const KamataEngine::WorldTransform& worldTransform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	KamataEngine::Camera& GetCamera() { return camera; }

	const KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const KamataEngine::WorldTransform* parent);

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera;

	KamataEngine::Vector3 translation_ = { 0.0f, 0.0f, 0.0f };

	KamataEngine::Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };
};
