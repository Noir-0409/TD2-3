#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "2D/Sprite.h"

class Fog {

public:

	void Initialize(KamataEngine::Sprite* sprite);

	void Update();

	void Draw();

private:

	KamataEngine::Sprite* sprite_ = nullptr;

};