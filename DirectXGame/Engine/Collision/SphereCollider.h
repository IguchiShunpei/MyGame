/**
* @file SphereCollider.h
* @brief 球の衝突判定オブジェクト
* @author イグチ_シュンペイ
*/

#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include "Vector3.h"
#include "Matrix4.h"


class SphereCollider :public BaseCollider, public Sphere
{
private:// エイリアス
public:
	SphereCollider(Vector3 offset = { 0,0,0 }, float radius = 1.0f) :offset_(offset), radius(radius) {
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	// 更新
	void Update()override;

	inline const Vector3& GetOffset() { return offset_; }

	inline void SetOffset(const Vector3& offset) { this->offset_ = offset; }

	inline float GetRadius() { return radius; }

	inline void SetRadius(float radius_) { this->radius = radius_; }

private:
	// オブジェクト中心からのオフセット
	Vector3 offset_;
	// 半径
	float radius;
};