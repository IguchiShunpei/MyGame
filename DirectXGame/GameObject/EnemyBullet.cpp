#include "EnemyBullet.h"
#include "SphereCollider.h"

EnemyBullet::~EnemyBullet()
{
	delete enemyBulletModel_;
}

void EnemyBullet::EnemyBulletInitialize(const Vector3& position, const Vector3& velocity)
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyBulletModel_ = Model::LoadFromOBJ("triangle_mat");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyBulletModel_);
	SetScale(Vector3(1.5f, 1.5f, 1.5f));
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	isDelete_ = false;
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
	worldTransform_.UpdateMatrix();

	//座標を移動させる
	worldTransform_.position_ -= velocity_;

	//時間経過で弾が消える
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void EnemyBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Player";
	const char* str2 = "class PlayerBullet";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0|| strcmp(toCollisionName, str2) == 0) {
		isDelete_ = true;
	}
}
