#pragma once

#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "3d/Model.h"

class PlayerAnime {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// Imguiの更新(デバッグのみ)
	/// </summary>
	void UpdateImgui();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

	KamataEngine::Vector3 GetWorldPosition();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;

	bool finished_ = false;


	// 終了までの時間
	float finishTimer_ = 0.0f;
	const float finishTime_ = 1.0f;

	// 自機の速度
	const float moveSpeed_ = 1.0f;

};

