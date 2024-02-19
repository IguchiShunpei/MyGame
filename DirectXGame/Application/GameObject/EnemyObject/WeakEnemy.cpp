/**
* @file WeakEnemy.cpp
* @brief 攻撃せずに移動するだけの敵クラス
* @author イグチ_シュンペイ
*/

#include "WeakEnemy.h"
#include "SphereCollider.h"
#include "GamePLayScene.h"
#include "string.h"

//デストラクタ
WeakEnemy::~WeakEnemy()
{
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
	SetModel(wEnemyModel.get());
	isDead_ = false;
	isInit_ = false;
	initTime_ = 60.0f;
}

void WeakEnemy::Update()
{
	enemyColor_ = { 1.0f,1.0f,1.0f };
	hitColor_ = { 3.0f,3.0f,3.0f };

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

void WeakEnemy::Damage(int damage)
{
  	enemyColor_ = hitColor_;
	hp_-= damage;
	if (hp_ <= 0)
	{
		isDead_ = true;
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
			Damage(damage_);
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

void WeakEnemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ + 120.0f * MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_++;
		if (initTime_ >= 60.0f)
		{
			isBack_ = true;
			isDelete_ = true;
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
