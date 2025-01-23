#pragma once
#include <3d\Camera.h>
#include <3d\Model.h>
#include <3d\WorldTransform.h>
#include <input\Input.h>
#include <2d\Sprite.h>
#include <base\TextureManager.h>

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

	float counter_ = 0.0f;
	// 終了フラグ
	bool finished_ = false;
	static inline const float kTimeTitleMove = 2.0f;

	// キーが反応するようになるまでの時間
	float canFinishCounter_ = 0.0f;
	const float canFinishCount_ = 0.2f;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;


};

