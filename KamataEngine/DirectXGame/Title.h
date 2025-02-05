#pragma once
#include <3d\Camera.h>
#include <3d\Model.h>
#include <3d\WorldTransform.h>
#include <input\Input.h>
#include <2d\Sprite.h>
#include <base\TextureManager.h>
#include "audio/Audio.h"
#include "Fade.h"

class Title
{
public:
	~Title();
	
	void Initialize();
	
	void Update();

	void Draw();

	// デスフラグの getter
	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Camera viewProjection_;
	KamataEngine::WorldTransform worldTransformTitle_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// フェード
	Fade* fade_ = nullptr;

	// 現在のフェーズ
	FadePhase phase_ = FadePhase::kFadeIn;

	float counter_ = 0.0f;
	// 終了フラグ
	bool finished_ = false;
	static inline const float kTimeTitleMove = 2.0f;

	// キーが反応するようになるまでの時間
	//float canFinishCounter_ = 0.0f;
	//const float canFinishCount_ = 0.2f;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t keyDataHandle = 0;
	uint32_t keyVoiceHandle = 0;

	uint32_t bgmDataHandle = 0;
	uint32_t bgmVoicehadle = 0;

};

