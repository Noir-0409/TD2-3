#include "Player.h"
#include <cassert>
#include "math/Vector3.h"
#include "MathUtility.h"
#include "Affin.h"
#include "imgui.h"

using namespace KamataEngine;

Player::~Player()
{

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle) {

	//NULLチェック
	assert(model);

	//引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得
	input_ = KamataEngine::Input::GetInstance();

}

void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {

		if (bullet->isDead()) {

			delete bullet;
			return true;

		}

		return false;

		}

	);

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kCharacterSpeed;

	}

	if (input_->PushKey(DIK_RIGHT)) {

		move.x += kCharacterSpeed;

	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {

		move.y += kCharacterSpeed;

	}

	if (input_->PushKey(DIK_DOWN)) {

		move.y -= kCharacterSpeed;

	}

	//座標移動
	worldTransform_.translation_ += move;

	//移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//行列更新
	worldTransform_.UpdateMatrix();

	Rotate();

	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {

		bullet->update();

	}

}

void Player::Draw(KamataEngine::Camera* camera) {

	//描画
	model_->Draw(worldTransform_, *camera, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {

		bullet->Draw(*camera);

	}

}

void Player::Rotate()
{

	//回転速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {

		//WorldTransformのY軸周り角度を回転速さ分減算
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_D)) {

		//WorldTransformのY軸周り角度を回転速さ分加算
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

void Player::Attack()
{

	if (input_->TriggerKey(DIK_RETURN)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成して初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録
		bullets_.push_back(newBullet);

	}

}

KamataEngine::Vector3 Player::GetWorldPosition()
{

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	// ワールド行列のTx
	worldPos.x = worldTransform_.translation_.x;

	// ワールド行列のTy
	worldPos.y = worldTransform_.translation_.y;

	// ワールド行列のTz
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;

}

void Player::OnCollision()
{
}
