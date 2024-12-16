#pragma once
#include <KamataEngine.h>
#include <3d/WorldTransform.h>
#include <3D/Model.h>

class Planet {

public:

	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);

	void Update();

	void Draw(KamataEngine::WorldTransform& worldTransform, KamataEngine::Camera& camera);

private:

//ワールドトランスフォーム
KamataEngine::WorldTransform worldTransform_;

//モデル
KamataEngine::Model* model_ = nullptr;

};