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
#include "EnemyTrackingBullet.h"
#include "Stars.h"
#include <sstream>
#include "Planets.h"
#include <chrono>
#include "Fade.h"

// 惑星
//enum class Planet {
//	normal, // ノーマル
//	control, // 操作反転
//	fog, // 視界悪化(霧)
//	//newEnemy, // 新しい敵
//	heal, // HP回復
//	damage, // HP減少
//	//attack, // 攻撃力変化
//	//bullet, // 弾の軌道
//	//obstacle, // 障害物
//	time, // 時間の流れ
//	gravity, // 重力
//
//};

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
	void AddEnemyTrackingBullet(EnemyTrackingBullet* enemyTrackingBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 惑星
	/// </summary>
	Planet GetPlanet() const { return planet_; }

	std::list<Enemy*>& GetEnemies() { return enemies_; }

	bool UseTarget() { return useTarget_; }
	// マウスの位置を取得
	KamataEngine::Vector2 GetMousePosition();
	// マウスの移動量の取得
	KamataEngine::Vector2 GetMousePos() const { return mousePos_; }

	//霧の濃さを変える
	void ChangeFogAlpha(float deltaTime);

	//瀕死エフェクトの濃さを変える
	void ChangeDedAlpha(float deltaTime);

	void RespawnEnemies();

	void OnPlanetChange();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	bool IsCleared() const {return isCleard_; }

private: // メンバ変数
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

	// 画面フェード
	Fade* fade_ = nullptr;
	FadePhase phase_ = FadePhase::kFadeIn;

	// 天球移動用
	KamataEngine::WorldTransform planetWorldTransform_;

	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelPlayerBullet_ = nullptr;

	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;
	std::list<EnemyTrackingBullet*> enemyTrackingBullets_;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }
	const std::list<EnemyTrackingBullet*>& GetEnemyTrackingBullets() const { return enemyTrackingBullets_; }

	// 天球
	Skydome* skyDome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	//霧
	KamataEngine::Sprite* fogSprite_ = nullptr;
	uint32_t fogTextureHandle_ = 0;

	//瀕死エフェクト
	KamataEngine::Sprite* dedSprite_ = nullptr;
	uint32_t dedTextureHandle_ = 0;

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

	//シーン終了フラグ
	bool finished_ = false;

	// 惑星
	Planets* planets_ = nullptr;
	KamataEngine::Model* modelPlanets_ = nullptr;
 
	bool isCleard_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Planet planet_ = Planet::normal;

	//霧の透明度
	float fogAlpha_ = 0.0f;
	float fogAlphaStep_ = 0.1f;

	//瀕死エフェクトの透明度
	float dedAlpha_ = 0.0f;
	float dedAlphaStep_ = 0.9f;

	std::chrono::steady_clock::time_point previousTime_;

	// 移動演出
	Stars* stars_ = nullptr;
	KamataEngine::Model* modelStars_;

	//クリアまでのタイマー
	float clearTimer_ =180.0f;

	Planet currentPlanet_ = Planet::normal;
	int spawnCooldown = 60;
	int spawnTimer = 0;

};