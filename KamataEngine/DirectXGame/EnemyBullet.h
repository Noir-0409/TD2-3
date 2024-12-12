#pragma once
#include <KamataEngine.h>
#include <3d/Model.h>
#include <math/Vector3.h>
#include <3d/Camera.h>

class EnemyBullet {

public:

	void initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void update();

	void Draw(const KamataEngine::Camera& camera);

	bool isDead() const { return isDead_; }

	//ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	KamataEngine::Vector3 velocity_;

	//消えるまでの時間
	static const int32_t kLifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};