#include "WeakEnemy.h"
#include "SphereCollider.h"
#include "GamePLayScene.h"
#include "string.h"

//デストラクタ
WeakEnemy::~WeakEnemy()
{
	delete wEnemyModel;
}

//初期化
void WeakEnemy::WEnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	wEnemyModel = Model::LoadFromOBJ("enemy02");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(wEnemyModel);
	isDead_ = false;
	isInit_ = false;
	initTime_ = 60.0f;
}

void WeakEnemy::Update()
{
	//登場モーション
	InitMotion();
	//登場したら移動
	if (isInit_ == true)
	{
		Move();
	}

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void WeakEnemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void WeakEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	if (isInit_ == true)
	{
		//相手がplayerBullet
		if (strcmp(toCollisionName, str1) == 0)
		{
			isHit_ = true;
			hp_--;
			if (hp_ == 0)
			{
				isDead_ = true;
			}
		}
	}
}

void WeakEnemy::Move()
{
	switch (phase_)
	{
	case Phase::None:
		break;
	case Phase::R:   //カーブフェーズ
		RCurve();
		break;
	case Phase::L:   //カーブフェーズ
		LCurve();
		break;
	}
}

void WeakEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + 60.0f * -MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_--;
		if (initTime_ <= 0.0f)
		{
			isInit_ = true;
		}
	}
}

void WeakEnemy::RCurve()
{
	MathFunc::CurveProjection(worldTransform_, startSpeed, C, flame);
}

void WeakEnemy::LCurve()
{
	MathFunc::CurveProjection(worldTransform_, { -startSpeed.x,startSpeed.y,startSpeed.z }, -C, flame);
}

Vector3 WeakEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}