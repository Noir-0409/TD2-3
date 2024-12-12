#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "3d/Model.h"
#include "math/MathUtility.h"
#include "kMath.h"
#include "base/TextureManager.h"
#include "EnemyBullet.h"
#include <list>

// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

// 前方宣言
class Player;
class GameScene;

class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

	void SetPlayer(Player* player) { player_ = player; }

	KamataEngine::Vector3 GetWorldPosition();

	// 発射間隔
	static const int kFireInterval = 60;

	void InitializeFirePhase();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	uint32_t textureHandle_ = 0;

	// 移動速度
	KamataEngine::Vector3 velocity_ = { 0.0f, 0.0f, 0.02f };


	// 行動
	void movePhase();
	Phase phase_ = Phase::Approach;
	KamataEngine::Vector3 ApproachVelocity = { 0.0f, 0.0f, -0.02f };
	KamataEngine::Vector3 LeaveVelocity = { -0.1f, 0.05f, 0.02f };

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 発射タイマー
	int32_t fireTimer_ = 0;


	// 自キャラ
	Player* player_ = nullptr;
};


