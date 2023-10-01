#include "InvEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
InvEnemy::~InvEnemy()
{
	delete enemyModel;
}

//初期化
void InvEnemy::InvEnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy03");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);
	isDelete_ = false;
	isHit_ = false;
}

void InvEnemy::Update()
{
	waitTimer++;
	if (waitTimer >= 120)
	{
		Move();
	}

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void InvEnemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void InvEnemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{

	}
}

void InvEnemy::Move()
{
	worldTransform_.position_.z -= 3.0f;
	if (worldTransform_.position_.z <= -20.0f)
	{
		isDelete_ = true;
	}
}


Vector3 InvEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
