#include "SceneTranslation.h"

SceneTranslation::SceneTranslation()
{
}

SceneTranslation::~SceneTranslation()
{

	delete fadeSprite_;


}

void SceneTranslation::Initialize()
{

	//初期化
	fadeTextureHandle_ = KamataEngine::TextureManager::Load("black.png");
	fadeSprite_ = KamataEngine::Sprite::Create(fadeTextureHandle_, { 0.0f,0.0f });

}

void SceneTranslation::Update(float deltaTime)
{

    if (deltaTime <= 0) {
        return; // deltaTime が不正な場合、処理を中断
    }

    // 徐々に透明度を増加させる（遷移開始時に呼び出す）
    if (fadeAlpha_ < 1.0f) {
        fadeAlpha_ += fadeSpeed_ * deltaTime;
        if (fadeAlpha_ > 1.0f) {
            fadeAlpha_ = 1.0f;  // 透明度の最大値
        }
    }
    // 徐々に透明度を減少させる（遷移終了時に呼び出す）
    else if (fadeAlpha_ > 0.0f) {
        fadeAlpha_ -= fadeSpeed_ * deltaTime;
        if (fadeAlpha_ < 0.0f) {
            fadeAlpha_ = 0.0f;  // 透明度の最小値
        }
    }

    // 現在の透明度をalpha_に格納
    alpha_ = fadeAlpha_;

}

void SceneTranslation::Draw()
{

    if (isTransitioning_) {
     
        fadeSprite_->Draw();  // スプライトを描画
    }

}

bool SceneTranslation::IsTransitionFinished() const
{
    return alpha_ == 0.0f && !isFadingOut_;
}

void SceneTranslation::SetNextScene(Scene nextScene)
{
    nextScene_ = nextScene;
}
