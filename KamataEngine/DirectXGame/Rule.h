#pragma once
#include <3d\Camera.h>
#include <3d\Model.h>
#include <3d\WorldTransform.h>
#include <input\Input.h>
#include <2d\Sprite.h>
#include <base\TextureManager.h>
#include "PlayerAnime.h"
#include "Fade.h"



class Rule
{
public:
	~Rule();
	
	void Initialize();
	
	void Update();

	void Draw();
	// デスフラグの getter
	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransformTitle_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::Model* modelPlayerAnime_ = nullptr;

	PlayerAnime* playerAnime_ = nullptr;

	Fade* fade_ = nullptr;

	FadePhase phase_ = FadePhase::kFadeIn;

	bool startAnimation_ = false;

	float counter_ = 0.0f;
	// 終了フラグ
	bool finished_ = false;
	static inline const float kTimeTitleMove = 2.0f;

	// キーが反応するようになるまでの時間
	float canFinishCounter_ = 0.0f;
	const float canFinishCount_ = 1.0f;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;


};

