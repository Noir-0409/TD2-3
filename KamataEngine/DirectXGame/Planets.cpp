#include "Planets.h"
#include "imgui.h"
#include "kMath.h"
#include <cassert>
#include <time.h>
#include <map>

using namespace KamataEngine;

std::map<int, Planet> planetTable = {
    {0, Planet::normal},
    {1, Planet::control},
	{2, Planet::fog},
	{3, Planet::heal},
	{4, Planet::damage},
	{5, Planet::attack},
	{6, Planet::obstacle},
	{7, Planet::time},
	{8, Planet::gravity},
};

std::map<int, sumLRUD> lrudTable = {
    {0, sumLRUD::right		},
    {1, sumLRUD::left		},
    {2, sumLRUD::rightUp	},
    {3, sumLRUD::leftUp		},
    {4, sumLRUD::rightDown	},
    {5, sumLRUD::leftDown	},
};

void Planets::Initialize(Model* model, Planet planet) { 
	srand(static_cast<unsigned int>(time(NULL)));
	assert(model);
	model_ = model;
	input_ = Input::GetInstance();
	//if (stage == "normal") {  // 惑星の初期化
	//	planet_ = Planet::normal;
	//	objectColor_->SetColor({1.0f, 1.0f, 1.0f, 0.3f});
	//} else if (stage == "control") {
	//	planet_ = Planet::control;
	//	objectColor_->SetColor({1.0f, 1.0f, 1.0f, 0.3f});
	//}
	planet_ = planet;
	objectColor_.Initialize();
	
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	objectColor_.SetColor(color_);
	worldTransform_.Initialize();
	worldTransform_.translation_ = {-400.0f, -50.0f, 800.0f};
	worldTransform_.rotation_.z -= 0.1f;
}

void Planets::UpdateImgui() { 
	ImGui::Begin("Planets");
	ImGui::ColorEdit4("color", &color_.x);
	ImGui::SliderFloat("changeTimer", &changePlanetsTimer_, 0.0f, changePlanetsTime_);
	ImGui::SliderFloat("stayTimer", &stayPlanetsTimer_, 0.0f, stayPlanetsTime_);
	ImGui::End();
	objectColor_.SetColor(color_);
}

bool Planets::animInPlanet(sumLRUD& lrud, float timer) {
	switch (lrud) {
	case sumLRUD::right:
		worldTransform_.translation_.x = 400.0f;
		worldTransform_.translation_.y = -50.0f;
		worldTransform_.translation_.z = easeInOut(timer, 1000.0f, 800.0f);
		color_.w = easeInOut(timer * 1.2f, 0.0f, 1.0f);
		break;
	case sumLRUD::left:
		worldTransform_.translation_.x = -400.0f;
		worldTransform_.translation_.y = -50.0f;
		worldTransform_.translation_.z = easeInOut(timer, 1000.0f, 800.0f);
		color_.w = easeInOut(timer * 1.2f, 0.0f, 1.0f);
		break;
	case sumLRUD::rightUp:
		worldTransform_.translation_.x = easeInOut(timer, 1000.0f, 400.0f);
		worldTransform_.translation_.y = easeInOut(timer, 650.0f, -50.0f);
		break;
	case sumLRUD::leftUp:
		worldTransform_.translation_.x = easeInOut(timer, -1000.0f, -400.0f);
		worldTransform_.translation_.y = easeInOut(timer, 650.0f, -50.0f);
		break;
	case sumLRUD::rightDown:
		worldTransform_.translation_.x = easeInOut(timer, 1000.0f, 400.0f);
		worldTransform_.translation_.y = easeInOut(timer, -650.0f, -50.0f);
		break;
	case sumLRUD::leftDown:
		worldTransform_.translation_.x = easeInOut(timer, -1000.0f, -400.0f);
		worldTransform_.translation_.y = easeInOut(timer, -650.0f, -50.0f);
		break;
	}
	if (timer >= 1.0f) {
		return true;
	}
	return false;
}

bool Planets::animOutPlanet(sumLRUD& lrud, float timer) {
	switch (lrud) {
	case sumLRUD::right:
		worldTransform_.translation_.x = easeInOut(timer, 400.0f, 600.0f);
		worldTransform_.translation_.z = easeInOut(timer, 800.0f, 0.0f);
		color_.w = easeInOut(timer * 2.0f, 1.0f, 0.0f);
		break;
	case sumLRUD::left:
		worldTransform_.translation_.x = easeInOut(timer, -400.0f, -600.0f);
		worldTransform_.translation_.z = easeInOut(timer, 800.0f, 0.0f);
		color_.w = easeInOut(timer * 2.0f, 1.0f, 0.0f);
		break;
	case sumLRUD::rightUp:
		worldTransform_.translation_.x = easeInOut(timer, 400.0f, 1000.0f);
		worldTransform_.translation_.y = easeInOut(timer, -50.0f, 650.0f);
		break;
	case sumLRUD::leftUp:
		worldTransform_.translation_.x = easeInOut(timer, -400.0f, -1000.0f);
		worldTransform_.translation_.y = easeInOut(timer, -50.0f, 650.0f);
		break;
	case sumLRUD::rightDown:
		worldTransform_.translation_.x = easeInOut(timer, 400.0f, 1000.0f);
		worldTransform_.translation_.y = easeInOut(timer, -50.0f, -650.0f);
		break;
	case sumLRUD::leftDown:
		worldTransform_.translation_.x = easeInOut(timer, -400.0f, -1000.0f);
		worldTransform_.translation_.y = easeInOut(timer, -50.0f, -650.0f);
		break;
	}
	if (timer >= 1.0f) {
		return true;
	}
	return false;
}

void Planets::Update() { 
#ifdef _DEBUG

#endif // _DEBUG
	if (!changePlanets_) {
		changePlanetsTimer_ += 1.0f / 60;
	}
	if (changePlanetsTimer_ >= changePlanetsTime_) { // 惑星切り替えタイマーが特定の時間を超えたら実行
		int planetNum = static_cast<int>(rand() % 9); // 惑星のタイプがランダムに選ばれる
		planet_ = planetTable[planetNum]; // 惑星タイプの代入
		int lrudNum = static_cast<int>(rand() % 2);
		if (planet_ == Planet::gravity) {
			lrudNum = static_cast<int>(rand() % 4) + 2;
		}
		lrud_ = lrudTable[lrudNum];
		changePlanetsTimer_ = 0.0f; // 惑星切り替えタイマーのリセット
		changePlanets_ = true; // 惑星切り替えタイマーを一時停止
		inAnimation_ = true;
		switch (planet_) {
		case Planet::normal:
			color_ = {1.0f, 1.0f, 1.0f, 0.0f};
			break;
		case Planet::control:
			color_ = {1.0f, 0.5f, 0.5f, 0.0f};
			break;
		case Planet::fog:
			color_ = {0.7f, 0.7f, 0.2f, 0.0f};
			break;
		case Planet::heal:
			color_ = {0.1f, 1.0f, 0.1f, 0.0f};
			break;
		case Planet::damage:
			color_ = {1.0f, 0.3f, 0.2f, 0.0f};
			break;
		case Planet::attack:
			color_ = {1.0f, 0.0f, 1.0f, 0.0f};
			break;
		case Planet::obstacle:
			color_ = {1.0f, 1.0f, 0.0f, 0.0f};
			break;
		case Planet::time:
			color_ = {0.0f, 1.0f, 1.0f, 0.0f};
			break;
		case Planet::gravity:
			color_ = {0.3f, 0.3f, 0.3f, 1.0f};
			break;
		}
	}


	if (inAnimation_) {
		animTimer_ += 1.0f / 60 / animTime_;
		if (animInPlanet(lrud_, animTimer_)) {
			inAnimation_ = false;
			stayPlanets_ = true;
			stayPlanetsTime_ = static_cast<float>(rand() % static_cast<int>(maxStayTime_)) + minStayTime_; // 惑星が留まる時間を指定された範囲内で算出(ランダム)
			animTimer_ = 0.0f;
		}
	}

	if (stayPlanets_) {
		stayPlanetsTimer_ += 1.0f / 60;
		if (stayPlanetsTimer_ >= stayPlanetsTime_) {
			outAnimation_ = true;
			stayPlanets_ = false;
			stayPlanetsTimer_ = 0.0f;
		}
	}
	if (outAnimation_) {
		animTimer_ += 1.0f / 60 / animTime_;
		if (animOutPlanet(lrud_, animTimer_)) {
			outAnimation_ = false;
			changePlanets_ = false;
			animTimer_ = 0.0f;
		}
	}
	
	objectColor_.SetColor(color_);

	worldTransform_.rotation_.y += 0.001f;
	
	worldTransform_.UpdateMatrix();
}

void Planets::Draw(Camera& camera) { model_->Draw(worldTransform_, camera, &objectColor_); }