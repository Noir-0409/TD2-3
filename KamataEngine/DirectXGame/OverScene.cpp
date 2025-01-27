#include "OverScene.h"
using namespace KamataEngine;

OverScene::~OverScene()
{
}

void OverScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	camera_.Initialize();

}

void OverScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		isFinished_ = true;

	}

}

void OverScene::Draw()
{
}
