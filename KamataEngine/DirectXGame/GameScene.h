#pragma once
#include <KamataEngine.h>
#include "Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "3d/DebugCamera.h"
#include "3d/Camera.h"

using namespace KamataEngine;

class GameScene {

	//メンバ関数
public:

	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

	//初期化
	void Initialze();

	//毎フレーム処理
	void Update();

	//描画
	void Draw();

	void CheckAllCollision();

	//メンバ変数
private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ビュープロジェクション(カメラ)
	Camera camera_;

	//プレイヤー
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	//天球モデル
	Model* modelSkydome_ = nullptr;

	//デバッグカメラ有効
	bool isDebugcameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

};