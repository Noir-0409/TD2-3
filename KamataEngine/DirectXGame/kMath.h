#pragma once

#include "AABB.h"
//#include "Matrix4x4.h"
#include <math\Vector3.h>
#include <math/Vector2.h>
#include <math\Matrix4x4.h>
//#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <cassert>
#include <cmath>
// 代入演算子オーバーロード
// Vector3の足し算
KamataEngine::Vector3& operator+=(KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhv);

KamataEngine::Vector3& operator*=(KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhv);

KamataEngine::Matrix4x4& operator*=(KamataEngine::Matrix4x4& hls, const KamataEngine::Matrix4x4& rhv);

KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

KamataEngine::Vector3 operator-(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

KamataEngine::Vector3& operator-=(KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

// 代入演算子オーバーロード
// Vector3の掛け算
KamataEngine::Vector3& operator*=(KamataEngine::Vector3& v, float s);

// 2項演算子オーバーロード
// Vector3の掛け算
const KamataEngine::Vector3 operator*(const KamataEngine::Vector3& v, float s);

// ease In-Out
float easeInOut(float t, float x1, float x2);

// Lerp
float Lerp(float x1, float x2, float t);

KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2, float t);

// 1, x軸回転行列
KamataEngine::Matrix4x4 MakeRotateXMatrix(float radian);

// 2, y軸回転行列
KamataEngine::Matrix4x4 MakeRotateYMatrix(float radian);

// 3, z軸回転行列
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian);

KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);

// ３次元アフィン変換行列
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);

// 3, 座標変換
KamataEngine::Vector3 MathTransform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);

// ベクトル変換
KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& v, const KamataEngine::Matrix4x4& m);

// 正規化Vector3
KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v);
// 正規化Vector2
KamataEngine::Vector2 Normalize(const KamataEngine::Vector2& v);

// 内積
float Dot(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

// 逆行列
// 逆行列
KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& m);