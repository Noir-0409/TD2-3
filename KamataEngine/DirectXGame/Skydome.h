#pragma once
#include <KamataEngine.h>
#include "3D/Model.h"
#include "3D/WorldTransform.h"

class Skydome {

public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();

	void Draw();

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera* camera_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

};