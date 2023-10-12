#include "BossEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
BossEnemy::~BossEnemy()
{
	delete enemyModel;
}

//初期化
void BossEnemy::BossEnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy04");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);
	isDead_ = false;
	isDeathTimer_ = false;

	deathTimer_ = 180;
}

void BossEnemy::Update()
{
	isHit_ = false;

	ActiveDeathTimer();

	Move();

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void BossEnemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void BossEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
		hp_--;
		if (hp_ == 0)
		{
			isDeathTimer_ = true;
		}
	}
}

void BossEnemy::Move()
{
	if (isDeathTimer_ == false)
	{
		switch (phase_)
		{
		case Phase::Approach: //接近フェーズ
		default:
			//移動(ベクトルを加算)
			Approach();
			break;
		case Phase::Leave:   //離脱フェーズ
			Leave();
			break;
		case Phase::Curve:   //カーブフェーズ
			Curve();
			break;
		case Phase::ReCurve:   //カーブフェーズ
			ReCurve();
			break;
		}
	}
}

void BossEnemy::ActiveDeathTimer()
{
	isHit_ = false;
	if (isDeathTimer_ == true)
	{
		if (deathTimer_ <= 0)
		{
			isDead_ = true;
		}
		deathTimer_--;
	}
}

void BossEnemy::Approach()
{
	worldTransform_.position_ += approach_;

	//既定の位置に着いたら離脱へ
	if (worldTransform_.position_.z <= 30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void BossEnemy::Leave()
{
	worldTransform_.position_ += leave_;

	//既定の位置に着いたらカーブへ
	if (worldTransform_.position_.z >= 100.0f)
	{
		phase_ = Phase::Approach;
	}
}

void BossEnemy::Curve()
{
	MathFunc::CurveProjection(worldTransform_, startSpeed, C, flame);
}

void BossEnemy::ReCurve()
{
	MathFunc::CurveProjection(worldTransform_, { -startSpeed.x,startSpeed.y,startSpeed.z }, -C, flame);
}


Vector3 BossEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
