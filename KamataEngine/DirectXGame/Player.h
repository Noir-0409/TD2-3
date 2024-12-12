#pragma once
#include "3D/Model.h"
#include "3D/WorldTransform.h"
#include "3D/camera.h"
#include <input/Input.h>
#include "PlayerBullet.h"
#include <list>

class Player {

public:

	~Player();

	void Initialize(KamataEngine::Model* model, uint32_t textureHandle);

	void Update();

	void Draw(KamataEngine::Camera* camera);

	void Rotate();

	void Attack();

	//ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キー入力
	KamataEngine::Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

};