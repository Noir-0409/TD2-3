#include "skydome.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera)
{
	worldTransform_.Initialize();
	model_ = model;
	camera_ = camera;
}

void Skydome::Update()
{
}

void Skydome::Draw()
{

	model_->Draw(worldTransform_, *camera_);

}
