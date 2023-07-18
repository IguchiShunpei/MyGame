#include "PlayerBullet.h"
#include "SphereCollider.h"

PlayerBullet::~PlayerBullet()
{
	delete playerBulletModel_;
}

void PlayerBullet::PlayerBulletInitialize(const Vector3& position, const Vector3& velocity)
{
	Initialize();
	// OBJからモデルデータを読み込む
	playerBulletModel_ = Model::LoadFromOBJ("ironSphere_01");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerBulletModel_);
	SetScale(Vector3(0.25f, 0.25f, 0.25f));
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	isDelete_ = false;
	chargeTime = 0;
}

void PlayerBullet::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}


void PlayerBullet::Update()
{
	if (isCharge_ == false)
	{
		if (bulletNum_ == 0)
		{
			worldTransform_.UpdateMatrix();
		}
		else if (bulletNum_ == 1)
		{
			MathFunc::HorizontalProjection(worldTransform_, startSpeed, G, flame);
		}
		else if (bulletNum_ == 2)
		{
			isCharge_ = true;
			worldTransform_.scale_ += scaleNum;
		}

		//座標を移動させる
		worldTransform_.position_ += velocity_;

		//時間経過で弾が消える
		if (--deleteTimer_ <= 0)
		{
			isDelete_ = true;
		}
	}
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Enemy";
	const char* str2 = "class EnemyBullet";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0) {
		isDelete_ = true;
	}
}
