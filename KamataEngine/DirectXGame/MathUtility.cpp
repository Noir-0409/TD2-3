#include "MathUtility.h"

using namespace KamataEngine;

//Vector3の加算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {

	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;

	return lhv;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {

	Vector3 temp(v1);
	return temp += v2;
}

//Vector3の減算
Vector3& operator-=(Vector3& lhv, const Vector3& rhv) {

	lhv.x -= rhv.x;
	lhv.y -= rhv.y;
	lhv.z -= rhv.z;

	return lhv;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {

	Vector3 temp(v1);

	return temp -= v2;
}

KamataEngine::Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m)
{

	Vector3 result{

		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]

	};

	return result;

}
