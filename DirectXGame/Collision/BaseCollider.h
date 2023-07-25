#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

//コライダー基底クラス
class BaseCollider
{
public:
	BaseCollider() = default;
	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object)
	{
		this->object3d = object;
		collname = object->GetName();
	}
	inline Object3d* GetObject3d()
	{
		return object3d;
	}
	void SetName(const char* name_) {
		this->collname = name_;
	}
	const char* GetName() {
		return collname;
	}

	//更新
	virtual void Update() = 0;

	//形状タイプ取得
	inline CollisionShapeType GetShapeType()
	{
		return shapeType;
	}

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d->OnCollision(info);
	}

	//衝突時コールバック関数
	inline void OffCollision(const CollisionInfo& info)
	{
		object3d->OffCollision(info);
	}

protected:
	Object3d* object3d = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//衝突先クラス名
	const char* collname = nullptr;
};