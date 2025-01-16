#pragma once
#include "KamataEngine.h"

class TitleScene {

public:

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() { return isFinished_; }

private:

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera camera_;

	bool isFinished_ = false;

};