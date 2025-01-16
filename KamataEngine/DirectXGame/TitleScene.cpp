#include "TitleScene.h"
using namespace KamataEngine;

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

}

void TitleScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		isFinished_ = true;

	}

}

void TitleScene::Draw()
{
}
