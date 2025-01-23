#pragma once
#include "3d/WorldTransform.h"
//#include <base\WinApp.h>
#include "3d/Camera.h"
#include "3d/Model.h"
#include <3d\ObjectColor.h>
#include <input\Input.h>
#include <math\Vector3.h>
#include "kMath.h"
#include "PlayerBullet.h"
#include <list>
#include <Windows.h>
#include <iostream>

class PlayerBullet;
class GameScene;

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
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

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const KamataEngine::WorldTransform* parent);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetTargetPositoin(KamataEngine::Vector3 targetPos) { targetWorldPosition_ = targetPos; }

	const KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	// ターゲットのワールド座標を取得
	KamataEngine::Vector3 GetTargetWorldPosition();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	bool IsDead() { return isDead_; }

	bool IsDelete() { return isDelete_; }

	bool IsDamage() { return isDamage_; }

	bool UseTarget() const { return useTarget_; }

	void SetUseTarget(bool useTarget) { useTarget_ = useTarget; }

	void UpdateImgui();

	bool IsTarget() const { return isTarget_; }

	void SetTarget(bool target) { isTarget_ = target; }

private:
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform targetWorldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* targetModel_ = nullptr;
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::ObjectColor targetObjectColor_;

	GameScene* gameScene_ = nullptr;

	// HP
	float hp_ = 100.0f;
	// 生きているかのフラグ
	bool isDead_ = false;

	// ダメージを受けたかどうか
	bool isDamage_ = false;
	// ダメージを受けた後の無敵時間
	float damageDelayTime_ = 1.0f;
	float damageDelayTimer_ = 0.0f;

	// ダメージを受けた時に敵を描画する時間
	int inDamageDrawCount_ = 1;
	// カウンター
	uint8_t inDamageDrawCounter_ = 0;

	void Move();
	void wolk();
	void Rotate();

	//KamataEngine::Vector2 GetMousePosition();
	KamataEngine::Vector2 mousePos_;

	// キャラクターの速さ
	const float kCharacterSpeed = 0.2f;
	// 移動限界座標
	const float kMoveLimitX = 1000.0f;
	const float kMoveLimitY = 1000.0f;

	/// キャラクターの回転
	// 回転の速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// プレイヤーの移動量
	float moveAmountZ_ = 0.0f;

	// 弾
	std::list<PlayerBullet*> bullets_;
	void Attack();

	// 攻撃間隔
	bool isAttack_ = false;
	float fireDelayTimer_ = 0.0f;
	float fireDelayTime_ = 0.3f;

	// 消滅までの時間
	float delTime_ = 0.0f;
	bool isDelete_ = false;

	// ターゲット機能のon/off
	bool useTarget_ = true;

	// 敵をターゲットしているか
	bool isTarget_ = false;

	void TargetUpdate();

	// ターゲットのワールド座標の取得
	KamataEngine::Vector3 targetWorldPosition_ = {0.0f, 0.0f, 0.0f};
	
};
