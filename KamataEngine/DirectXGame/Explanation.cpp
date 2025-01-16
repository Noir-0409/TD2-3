#include "Explanation.h"
using namespace KamataEngine;

void ExplanationScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

}

void ExplanationScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		isFinished_ = true;

	}

}

void ExplanationScene::Draw()
{
}
