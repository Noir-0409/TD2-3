#pragma once
#include "2d/Sprite.h"
#include "base/TextureManager.h"

// シーンのフェーズ
enum class FadePhase {
	kFadeIn,  // フェードイン
	kMain,    // メイン部
	kFadeOut, // フェードアウト
};

class Fade {
public:
	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

	// フェード開始
	void Start(Status status, float duration);

	// フェード終了判定
	bool IsFinished() const;

	// フェード終了
	void Stop();

private:
	// スプライトの宣言
	KamataEngine::Sprite* sprite_ = nullptr;

	// テクスチャ
	uint32_t fadeTexture_ = 0;

	// 現在のフェードの状態
	Status status_ = Status::None;

	// フェードの持続時間
	float duration_ = 0.0f;

	// 経過時間カウンター
	float counter_ = 0.0f;

};
