#include "Item.h"
#include "SphereCollider.h"
#include "string.h"

Item::~Item()
{
	delete itemModel_;
}

void Item::ItemInitialize(Vector3 initPos_)
{
	Initialize();
	// OBJからモデルデータを読み込む
	itemModel_ = Model::LoadFromOBJ("item");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(itemModel_);
	SetPosition(initPos_);
	isDelete_ = false;
	isHit_ = false;

	//通常
	moveSpeed_ = 0.2f;
	normalRotaSpeed_ = 3.0f;
	frontZ_ = -100.0f;

	//ヒット
	hitRotaSpeed_ = 6.0f;
	hitScale_ = { 5.0f,5.0f,5.0f };
	HitMotionTime_ = 0.0f;
	HitMotionTimeMax_ = 20.0f;
}

void Item::ItemUpdate()
{
	Move();
	Rotate();
	Hit();

	Update();
}

void Item::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Item::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class Player";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
	}
}

void Item::Move()
{
	worldTransform_.position_.z -= moveSpeed_;
	//カメラよりも奥に進んだら削除
	if (worldTransform_.position_.z <= frontZ_)
	{
		isDelete_ = true;
	}
}

void Item::Rotate()
{
	if (isHit_ == false)
	{
		worldTransform_.rotation_.y += normalRotaSpeed_;
	}
	else
	{
		worldTransform_.rotation_.y += hitRotaSpeed_;
	}
}

void Item::Hit()
{
	if (isHit_ == true)
	{
		HitMotion();
	}
}

void Item::HitMotion()
{
	if (HitMotionTime_ < HitMotionTimeMax_)
	{
		worldTransform_.scale_.x = hitScale_.x + 5.0f * -MathFunc::easeOutSine(HitMotionTime_ / HitMotionTimeMax_);
		worldTransform_.scale_.y = hitScale_.y + 5.0f * -MathFunc::easeOutSine(HitMotionTime_ / HitMotionTimeMax_);
		worldTransform_.scale_.z = hitScale_.z + 5.0f * -MathFunc::easeOutSine(HitMotionTime_ / HitMotionTimeMax_);
		HitMotionTime_++;
	}
	else
	{
		isDelete_ = true;
	}
}
