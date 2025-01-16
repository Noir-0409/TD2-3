#include "ResultScene.h"
using namespace KamataEngine;

void ResultScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();


}

void ResultScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		isFinished_ = true;

	}

}

void ResultScene::Draw()
{
}
