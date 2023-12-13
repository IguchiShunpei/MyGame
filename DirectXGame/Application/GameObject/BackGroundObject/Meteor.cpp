#include "Meteor.h"
#include "GamePlayScene.h"
#include "SphereCollider.h"
#include "string.h"

void Meteor::MeteorInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	meteorModel_ = Model::LoadFromOBJ("meteor");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(meteorModel_);

	speed_ = 0.2f;
	frontZ_ = -100.0f;

	rotaSpeed_ = 1.0f;
	size_ = Vector3(5.0f, 5.0f, 5.0f);

	hp_ = 3;

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	//ランダムで回転方向を割り当てる
	std::uniform_int_distribution<> isRotation(0, 1);
	isRota_ = isRotation(engine);

	isHit_ = false;
	isHitEnd_ = true;
}

void Meteor::MeteorUpdate()
{
	if (isDead_ == false)
	{
		color_ = { 1.0f,1.0f,1.0f };
		//移動
		Move();
		//回転
		Rotate();
		//当たり判定更新
		ColliderUpdate();
		//ダメージ
		Damage();
	}
	//更新
	Update();
}

void Meteor::Move()
{
	//移動
	worldTransform_.position_.z -= speed_;

	//カメラよりも奥に進んだら削除
	if (worldTransform_.position_.z <= frontZ_)
	{
		isDelete_ = true;
	}
}

void Meteor::Rotate()
{
	//基準のサイズよりも小さい物は回転させる
	if (isRota_ == 1)
	{
		worldTransform_.rotation_.x -= rotaSpeed_;
	}
}

void Meteor::Damage()
{
	if (isHit_ == true)
	{
		hp_--;
		color_ = { 3.0f,3.0f,3.0f };
		if (hp_ <= 0)
		{
			isDead_ = true;
		}
	}
	isHit_ = false;
}

void Meteor::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Meteor::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
	}
}

