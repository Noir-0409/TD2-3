#pragma once
#include "3d/WorldTransform.h"
//#include <base\WinApp.h>
#include "3d/Camera.h"
#include "3d/Model.h"
#include <3d\ObjectColor.h>
#include <input\Input.h>
#include <math\Vector3.h>
#include "kMath.h"
#include <2d\Sprite.h>
#include <base\TextureManager.h>
#include "3d/ObjectColor.h"

class PlayerBullet {
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

	// getter
	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition();

private:
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Camera camera_;
	KamataEngine::Model* model_ = nullptr;

	// 移動速度
	KamataEngine::Vector3 velocity_;

	// 消滅までの時間(寿命)<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
