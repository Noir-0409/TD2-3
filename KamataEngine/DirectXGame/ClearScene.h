#pragma once
#include <KamataEngine.h>
#include "base/DirectXCommon.h"
#include "input/Input.h"

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

	bool isFinished_ = false;

};