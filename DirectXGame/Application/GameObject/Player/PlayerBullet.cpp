/**
* @file PlayerBullet.cpp
* @brief 自機の弾の処理クラス
* @author イグチ_シュンペイ
*/

#include "PlayerBullet.h"
#include "SphereCollider.h"

PlayerBullet::~PlayerBullet()
{
	delete playerBulletModel_;
}

void PlayerBullet::PlayerBulletInitialize(const Vector3& position, const Vector3& velocity, int bulletDir,int bulletLevel)
{
	Initialize();
	// OBJからモデルデータを読み込む
	switch (bulletLevel)
	{
	case 1:
		playerBulletModel_ = Model::LoadFromOBJ("PlayerBullet01");
		break;
	case 2:
		playerBulletModel_ = Model::LoadFromOBJ("PlayerBullet02");
		break;
	}
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerBulletModel_);
	SetScale(Vector3(1.5f, 1.5f, 1.5f));
	if (bulletDir == 0)
	{
		SetRotation(Vector3(0.0f, -180.0f, 0.0f));
	}
	else
	{
		SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	}
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	isDelete_ = false;
	chargeTime = 0;
	deleteTimer_ = 120.0f;
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
	worldTransform_.UpdateMatrix();

	if (worldTransform_.scale_.x >= 0.7f)
	{
		worldTransform_.scale_ -= Vector3(0.3f, 0.3f, 0.3f);
	}

	//座標を移動させる
	worldTransform_.position_ += velocity_;

	//時間経過で弾が消える
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void PlayerBullet::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class Enemy";
	const char* str2 = "class WeakEnemy";
	const char* str3 = "class EnemyBullet";
	const char* str4 = "class InvEnemy";
	const char* str5 = "class Meteor";
	const char* str6 = "class BossEnemy";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0 ||
		strcmp(toCollisionName, str3) == 0 || strcmp(toCollisionName, str4) == 0 ||
		strcmp(toCollisionName, str5) == 0 || strcmp(toCollisionName, str6) == 0)
	{
		isDelete_ = true;
	}
}
