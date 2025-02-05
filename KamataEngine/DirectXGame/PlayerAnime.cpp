#include "PlayerAnime.h"
#include "kMath.h"
#include <imgui.h>
#include <cassert>

using namespace KamataEngine;

// ワールド座標を取得
Vector3 PlayerAnime::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド座標の平行同成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerAnime::Initialize(Model* model, Vector3 position) { 
	assert(model);
	model_ = model;

	finished_ = false;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerAnime::Update() {

	Vector3 velocity = {0.0f, 0.0f, moveSpeed_};
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.translation_ += velocity;

	worldTransform_.UpdateMatrix();

	finishTimer_ += 1.0f / 60;
	if (finishTimer_ >= finishTime_) {
		finishTimer_ = 0.0f;
		finished_ = true;
	}
}

void PlayerAnime::UpdateImgui() { 
	/*ImGui::Begin("PlayerAnimeState");
	ImGui::SliderFloat3("rotate", &worldTransform_.rotation_.x, -3.2f, 3.2f);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();*/

	worldTransform_.UpdateMatrix();
}

void PlayerAnime::Draw(Camera& camera) { 
	model_->Draw(worldTransform_, camera);
}