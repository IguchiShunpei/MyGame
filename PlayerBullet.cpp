#include "PlayerBullet.h"

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
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	isDelete_ = false;
}

void PlayerBullet::Update()
{
	worldTransform_.UpdateMatrix();

	//座標を移動させる
	worldTransform_.position_ += velocity_;

	//時間経過で弾が消える
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Enemy";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0) {
		isDelete_ = true;
	}
}
