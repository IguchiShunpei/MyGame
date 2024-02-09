/**
* @file PlayerBullet.cpp
* @brief 自機の弾の処理クラス
* @author イグチ_シュンペイ
*/

#include "PlayerBullet.h"
#include "SphereCollider.h"

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::PlayerBulletInitialize(const Vector3& position, int bulletLevel,float lengthZ)
{
	Initialize();
	// OBJからモデルデータを読み込む
	playerBulletModel_ = Model::LoadFromOBJ("PlayerBullet01");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerBulletModel_.get());
	SetScale(Vector3(1.0f, 1.0f, lengthZ));
	bulletColor_ = {1.0f,1.0f,1.0f};
	if (bulletLevel == 2)
	{
		bulletColor_ = { 1.0f,0.0f,0.0f };
	}
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
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
	if (worldTransform_.scale_.x >= 0.0f)
	{
		worldTransform_.scale_ -= Vector3(0.3f, 0.3f, 0.3f);
	}

	//回転
	worldTransform_.rotation_.z += 10.0f;

	//時間経過で弾が消える
	if (worldTransform_.scale_.x <= 0.0f)
	{
		isDelete_ = true;
	}
 	worldTransform_.UpdateMatrix();
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

float PlayerBullet::GetAngle(float mx, float my, float sx, float sy)
{
	float angle = atan2f(float(sy - my), float(sx - mx));
	return angle;
}
