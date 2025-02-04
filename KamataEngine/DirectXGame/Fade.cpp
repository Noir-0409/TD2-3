#include "Fade.h"
#include "base/WinApp.h"
#include <algorithm>

using namespace KamataEngine;

void Fade::Initialize() {
	fadeTexture_ = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(fadeTexture_, Vector2{0, 0});
	sprite_->SetSize(Vector2{WinApp::kWindowWidth, WinApp::kWindowHeight});
	sprite_->SetColor(Vector4{0, 0, 0, 1});
}

void Fade::Update() {
	// フェード状態による分岐
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		// フェードイン中の更新処理
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4{0, 0, 0, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)});
		break;
	case Status::FadeOut:
		// フェードアウト中の更新処理
		// フェードイン中の更新処理
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4{0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)});
		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {
	if (status_ == Status::None) {
		return;
	}


#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
#pragma endregion
}

void Fade::Start(Status status, float duration) { 
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

bool Fade::IsFinished() const {
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}

void Fade::Stop() { status_ = Status::None; }