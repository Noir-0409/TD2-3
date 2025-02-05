#include "Title.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include <cmath>
#include <numbers>

Title::~Title() {
	delete fade_;
	delete model_;
	delete sprite_;

	audio_->StopWave(bgmVoicehadle);

}

void Title::Initialize()
{

	audio_ = KamataEngine::Audio::GetInstance();

	viewProjection_.Initialize();
	finished_ = false;
	textureHandle_ = KamataEngine::TextureManager::Load("./Resources/Scene/title.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, { 0, 0 });

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	bgmDataHandle = audio_->LoadWave("./Resources./sound./scene./main./BGM5.wav");
	bgmVoicehadle = audio_->PlayWave(bgmDataHandle, true, 0.3f);

}

void Title::Update() {
	switch (phase_) {
	case FadePhase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kMain;
		}
		break;
	case FadePhase::kMain:

		if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)/* && canFinishCounter_ >= canFinishCount_*/) {
			//canFinishCounter_ = 0.0f;
			phase_ = FadePhase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case FadePhase::kFadeOut:
		if (fade_->IsFinished()) {
			phase_ = FadePhase::kFadeIn;
			finished_ = true;
		}
		break;
	}
	
	fade_->Update();
}

void Title::Draw() {
	KamataEngine::DirectXCommon* dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);
	sprite_->Draw();
	fade_->Draw(commandList);

	KamataEngine::Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
}