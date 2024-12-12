#pragma once
#include "3D/Model.h"
#include "3D/WorldTransform.h"
#include "3D/camera.h"
#include "EnemyBullet.h"

class Player;

class Enemy {

	//行動フェーズ
	enum class Phase {

		Approach, //接近
		Leave //離脱

	};

public:

	~Enemy();

	void Initialze(KamataEngine::Model* model, uint32_t textureHandle, const KamataEngine::Vector3& position);

	void Update();

	void Draw(KamataEngine::Camera* camera);

	// 接近フェーズの処理
	void Approach();

	//接近フェーズ初期化
	void ApproachInitialize();

	// 離脱フェーズの処理
	void Leave();

	void Fire();

	//発射間隔
	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

};