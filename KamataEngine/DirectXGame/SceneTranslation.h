#pragma once
#include "Scene.h"
#include <2d/Sprite.h>
#include <KamataEngine.h>

class SceneTranslation {

public:

	SceneTranslation();
	~SceneTranslation();

	void Initialize();

	void Update(float deltaTime);

	void Draw();

	bool IsTransitionFinished() const;

	void SetNextScene(Scene nextScene);	

private:

	KamataEngine::Sprite* fadeSprite_ = nullptr;
	uint32_t fadeTextureHandle_ = 0;

	bool isTransitioning_ = false;
	bool isFadingOut_ = false;
	bool isFadingIn_ = false; 
	
	float alpha_ = 0.0f;
	float fadeAlpha_ = 0.0f;
	float fadeSpeed_ = 1.0f;
	
	Scene nextScene_;

};