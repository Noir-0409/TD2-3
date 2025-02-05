#pragma once

#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/ObjectColor.h"
#include "input/Input.h"

// 惑星
enum class Planet {
	normal,   // ノーマル
	control,  // 操作反転
	fog,      // 視界悪化(霧)
	newEnemy, // 新しい敵
	//heal,     // HP回復
	//damage,   // HP減少
//	attack,   // 攻撃力変化
	//bullet,   // 弾の軌道
	obstacle, // 障害物
	time,     // 時間の流れ
	gravity,  // 重力

};

enum class sumLRUD {
	right,		// 右
	left,       // 左
	rightUp,	// 右上
	leftUp,		// 左上
	rightDown,	// 右下
	leftDown,	// 左下
};

class Planets {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, Planet planet);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

	void UpdateImgui();

	Planet GetPlanet() const { return planet_; }

	bool animInPlanet(sumLRUD& lrud, float timer);

	bool animOutPlanet(sumLRUD& lrud, float timer);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_;
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_;
	KamataEngine::Input* input_ = nullptr;

	Planet planet_ = Planet::normal;

	// 惑星がやってくる方向
	sumLRUD lrud_ = sumLRUD::left;

	// 次の惑星に変わるまでの時間
	float changePlanetsTimer_ = 0.0f;
	float changePlanetsTime_ = 5.0f;

	bool changePlanets_ = false;

	// 惑星の留まる時間
	float stayPlanetsTimer_ = 0.0f;
	float stayPlanetsTime_ = 0.0f;

	// 惑星が留まっている時のフラグ
	float stayPlanets_ = false;

	// 惑星が留まる時間の最大と最小値
	float maxStayTime_ = 5.0f;
	float minStayTime_ = 1.0f;

	// アニメーション中のフラグ
	bool inAnimation_ = false;
	bool outAnimation_ = false;

	// アニメーション
	float animTimer_ = 0.0f;
	float animTime_ = 7.0f;
};
