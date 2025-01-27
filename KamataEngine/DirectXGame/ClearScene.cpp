#include "ClearScene.h"

using namespace KamataEngine;

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	camera_.Initialize();

}

void ClearScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		isFinished_ = true;

	}

}

void ClearScene::Draw()
{
}
