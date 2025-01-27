#include "Title.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include <cmath>
#include <numbers>

Title::~Title() {
	delete model_;
	delete sprite_;

}

void Title::Initialize()
{
	viewProjection_.Initialize();
	finished_ = false;
	textureHandle_ = KamataEngine::TextureManager::Load("./Resources/Scene/title.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, { 0, 0 });

}

void Title::Update() {

	canFinishCounter_ += 1.0f / 60;

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) && canFinishCounter_ >= canFinishCount_) {
		canFinishCounter_ = 0.0f;
		finished_ = true;
	}

}

void Title::Draw() {
	KamataEngine::DirectXCommon* dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);
	sprite_->Draw();

	KamataEngine::Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
}