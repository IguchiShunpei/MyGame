/**
* @file EnemyBullet.cpp
* @brief 敵の弾の処理クラス
* @author イグチ_シュンペイ
*/

#include "EnemyBullet.h"
#include "SphereCollider.h"

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::EnemyBulletInitialize(const Vector3& position, const Vector3& velocity)
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyBulletModel_ = Model::LoadFromOBJ("EnemyBullet");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyBulletModel_.get());
	SetScale(Vector3(0.5f, 0.5f, 1.5f));
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	isDelete_ = false;
	deleteTimer_ = 300.0f;
	color_ = { 3.0f,1.0f,1.0f };
	rota_ = 5.0f;
}

void EnemyBullet::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}


void EnemyBullet::Update()
{
	//座標を移動させる
	worldTransform_.position_ -= velocity_;
	worldTransform_.rotation_.z += rota_;
	worldTransform_.UpdateMatrix();

	//時間経過で弾が消える
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void EnemyBullet::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class Player";
	const char* str2 = "class PlayerBullet";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0) 
	{
		isDelete_ = true;
	}
}
