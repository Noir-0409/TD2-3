#include "Rule.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include <cmath>
#include <numbers>

Rule::~Rule()
{
	delete model_;
	delete sprite_;
}

void Rule::Initialize() {
	
	viewProjection_.Initialize();
	finished_ = false;
	textureHandle_ = KamataEngine::TextureManager::Load("./Resources/Scene/rule.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, { 0, 0 });
}

void Rule::Update() {

	if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

}

void Rule::Draw() {
	KamataEngine::DirectXCommon* dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);
	sprite_->Draw();

	KamataEngine::Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
}