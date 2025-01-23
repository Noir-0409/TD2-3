#include "Rule.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include <cmath>
#include <numbers>

using namespace KamataEngine;

Rule::~Rule()
{
	delete modelPlayerAnime_;
	delete sprite_;
	delete playerAnime_;
}

void Rule::Initialize() {
	
	camera_.Initialize();
	finished_ = false;
	textureHandle_ = KamataEngine::TextureManager::Load("./Resources/Scene/rule.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, { 0, 0 });
	// プレイヤーの初期化
	modelPlayerAnime_ = KamataEngine::Model::CreateFromOBJ("enemy");

	playerAnime_ = new PlayerAnime();
	playerAnime_->Initialize(modelPlayerAnime_, Vector3{0.8f, -0.6f, -48.0f});
}

void Rule::Update() {
#ifdef _DEBUG
	playerAnime_->UpdateImgui();
#endif // _DEBUG


	canFinishCounter_ += 1.0f / 60;

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) && canFinishCounter_ >= canFinishCount_) {
		startAnimation_ = true;
		/*canFinishCounter_ = 0.0f;
		finished_ = true;*/
	}
	if (startAnimation_) {
		playerAnime_->Update();
	}
	if (playerAnime_->IsFinished()) {
		startAnimation_ = false;
		finished_ = true;
	}

}

void Rule::Draw() {
	KamataEngine::DirectXCommon* dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	KamataEngine::Sprite::PreDraw(commandList);
	sprite_->Draw();

	KamataEngine::Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();

	#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	playerAnime_->Draw(camera_);

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion
}