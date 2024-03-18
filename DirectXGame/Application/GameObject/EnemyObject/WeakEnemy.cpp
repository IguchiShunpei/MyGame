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
	wEnemyModel = Model::LoadFromOBJ("WeakEnemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(wEnemyModel.get());
	isDead_ = false;
	isInit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 90.0f;
	rota_ = 5.0f;
	moveY_ = 60.0f;
	afterMoveY_ = -60.0f;
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
	case Phase::R:  
		worldTransform_.rotation_.y -= rota_;
		MathFunc::CurveProjection(worldTransform_, startSpeedBeside, C, flame);
		break;
	case Phase::L:   //カーブフェーズ
		worldTransform_.rotation_.y += rota_;
		MathFunc::CurveProjection(worldTransform_, { -startSpeedBeside.x,startSpeedBeside.y,startSpeedBeside.z }, -C, flame);
		break;
	case Phase::U:   //カーブフェーズ
		worldTransform_.rotation_.x += rota_;
		MathFunc::HorizontalProjection(worldTransform_, { startSpeedVertical.x,-startSpeedVertical.y,startSpeedVertical.z }, -C, flame);
		break;
	case Phase::D:   //カーブフェーズ
		worldTransform_.rotation_.x -= rota_;
		MathFunc::HorizontalProjection(worldTransform_, startSpeedVertical, C, flame);
		break;
	}
}

void WeakEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + (afterMoveY_ + (moveY_ * MathFunc::easeOutSine(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isInit_ = true;
			initTime_ = 0.0f;
		}
	}
}

void WeakEnemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ - moveY_ * MathFunc::easeInSine(initTime_ / initTimeMax_);
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
}

Vector3 WeakEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
