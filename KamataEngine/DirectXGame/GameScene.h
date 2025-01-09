#pragma once

#include "3d/WorldTransform.h"
#include "audio/Audio.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include "3d/Model.h"
#include "2d/Sprite.h"
#include "3d/Camera.h"
#include "3d/WorldTransform.h"
#include <math\MathUtility.h>
#include <math\Matrix4x4.h>
#include <math\Vector3.h>
#include <math\Vector4.h>
#include <cmath>
#include "3d/DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// ロックオン
	/// </summary>
	void CheckLockOn();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	bool UseTarget() { return useTarget_; }
	// マウスの位置を取得
	KamataEngine::Vector2 GetMousePosition();
	// マウスの移動量の取得
	KamataEngine::Vector2 GetMousePos() const { return mousePos_; }

private: // メンバ変数
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	// 天球
	Skydome* skyDome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 敵発生コマンドの待機
	bool waitFlag = false;
	int32_t waitTimer = 0;

	// マウスカーソルの表示非表示
	bool showCursor_ = false;
	int cursor = ShowCursor(showCursor_);

	bool showMenu_ = false;

	bool useTarget_ = false;

	// ロックオンタイマー
	float lockonTimer_ = 0.0f;
	float lockonTime_ = 1.0f;

	KamataEngine::Vector2 mousePos_;

	// カーソル処理の更新
	void UpdateCursor();

	// マウス感度
	KamataEngine::Vector2 mouseSensi_; 

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};