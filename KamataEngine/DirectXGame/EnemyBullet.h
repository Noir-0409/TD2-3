#pragma once
#include "3d/WorldTransform.h"
#include "3d/Model.h"
#include "3d/Camera.h"
#include "base/TextureManager.h"
#include "kMath.h"
#include "math/MathUtility.h"
#include "3d/ObjectColor.h"

class EnemyBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::ObjectColor objectColor_;

	uint32_t textureHandle_;

	// 移動速度
	KamataEngine::Vector3 velocity_;

	// 消滅までの時間(寿命)<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};
