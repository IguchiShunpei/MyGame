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
	isHit_ = false;
	if (isTurn_ == false && isStart_ == false)
	{
		waitTimer++;
		if (waitTimer >= 180)
		{
			isTurn_ = true;
		}
	}
	Turn();
	Move();

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

void InvEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
	}
}

void InvEnemy::Move()
{
	if (isStart_ == true)
	{
		worldTransform_.position_.z -= 3.0f;
		if (worldTransform_.position_.z <= -20.0f)
		{
			isDelete_ = true;
		}
	}
}

void InvEnemy::Turn()
{
	if (isTurn_ == true)
	{
		turnTimer_++;
		worldTransform_.rotation_.z = 360.0f * MathFunc::easeOutSine(turnTimer_ / 30.0f);
		if (turnTimer_ >= 30.0f)
		{
			worldTransform_.rotation_.z = 360.0f;
			isTurn_ = false;
			isStart_ = true;
		}
	}
}

void InvEnemy::InitMotion()
{

}


Vector3 InvEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
