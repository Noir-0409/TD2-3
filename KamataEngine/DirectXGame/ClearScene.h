#pragma once
#include <KamataEngine.h>
#include "base/DirectXCommon.h"
#include "input/Input.h"
#include "Fade.h"

class ClearScene {

public:

	~ClearScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() { return isFinished_; }

private:

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	KamataEngine::Camera camera_;

	Fade* fade_ = nullptr;
	FadePhase phase_ = FadePhase::kFadeIn;

	bool isFinished_ = false;

};