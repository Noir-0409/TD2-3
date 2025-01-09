#include "RailCamera.h"
#include "imgui.h"

using namespace KamataEngine;

void RailCamera::Initialize(const WorldTransform& worldTransform) {
	// ワールドトランスフォームの初期化
	worldTransform_.matWorld_ = worldTransform.matWorld_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	// カメラの初期化
	camera.Initialize();
}

void RailCamera::Update() {
	worldTransform_.translation_ = worldTransform_.parent_->translation_;
	worldTransform_.rotation_ = worldTransform_.parent_->rotation_;

	worldTransform_.translation_ += translation_;
	worldTransform_.rotation_ += rotation_;
	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera.matView = Inverse(worldTransform_.matWorld_);
}

void RailCamera::UpdateImgui() {
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation", &translation_.x, 0.1f);
	ImGui::DragFloat3("Rotation", &rotation_.x, 0.1f);
	ImGui::End();
	//camera.UpdateMatrix();
#endif // _DEBUG
}

void RailCamera::SetParent(const WorldTransform* parent) {
	// 親関係を結ぶ
	worldTransform_.parent_ = parent;
}