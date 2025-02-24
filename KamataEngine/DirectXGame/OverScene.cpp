#include "OverScene.h"
using namespace KamataEngine;

OverScene::~OverScene()
{
	delete fade_;
	delete overSprite_;

}

void OverScene::Initialize()
{
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	camera_.Initialize();

	overTextureHandle_ = TextureManager::Load("./Resources/Scene/gameover.png");
	overSprite_ = Sprite::Create(overTextureHandle_, { 0.0f,0.0f });

}

void OverScene::Update()
{
	switch (phase_) {
	case FadePhase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kMain;
		}
		break;
	case FadePhase::kMain:
		if (input_->TriggerKey(DIK_RETURN)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = FadePhase::kFadeOut;
		}
		break;
	case FadePhase::kFadeOut:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kFadeIn;
			isFinished_ = true;
		}
		break;
	}
	fade_->Update();
}

void OverScene::Draw()
{
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);

	overSprite_->Draw();

	KamataEngine::Sprite::PostDraw();

	KamataEngine::Sprite::PreDraw(commandList);
	fade_->Draw(commandList);

	KamataEngine::Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
}
