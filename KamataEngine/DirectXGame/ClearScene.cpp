#include "ClearScene.h"

using namespace KamataEngine;

ClearScene::~ClearScene()
{ 
	delete fade_;
	delete clearSprite_;

}

void ClearScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	camera_.Initialize();

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	clearTextureHandle_ = TextureManager::Load("./Resources/Scene/clear.png");
	clearSprite_ = Sprite::Create(clearTextureHandle_, { 0.0f,0.0f });

}

void ClearScene::Update()
{

	switch (phase_) {
	case FadePhase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kMain;
		}
		break;
	case FadePhase::kMain:
		if (input_->TriggerKey(DIK_RETURN)) {
			phase_ = FadePhase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case FadePhase::kFadeOut:
		if (fade_->IsFinished()) {
			isFinished_ = true;
			phase_ = FadePhase::kFadeIn;
		}
		break;
	}
	fade_->Update();
}

void ClearScene::Draw()
{
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);

	clearSprite_->Draw();

	KamataEngine::Sprite::PostDraw();

	KamataEngine::Sprite::PreDraw(commandList);
	fade_->Draw(commandList);

	KamataEngine::Sprite::PostDraw();

	dxCommon_->ClearDepthBuffer();
}
