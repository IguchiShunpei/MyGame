/**
* @file Enemy.cpp
* @brief 汎用的な敵の処理クラス
* @author イグチ_シュンペイ
*/

#include "Enemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"
#include "Vector3.h"

//デストラクタ
Enemy::~Enemy()
{

}

//初期化
void Enemy::EnemyInitialize()
{
	Initialize();

	isDead_ = false;
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 180.0f;
	initY_ = 60.0f;
	afterInitY_ = 60.0f;

	enemyColor_ = { 0.0f,0.0f,0.0f };
	originalColor_ = { 1.0f,1.0f,1.0f };
	changeColor_ = { 3.0f,3.0f,3.0f };
}

void Enemy::EnemyUpdate()
{
	isHit_ = false;
	enemyColor_ = originalColor_;
	changeColor_ = { 3.0f,3.0f,3.0f };

	//登場モーション
	InitMotion();

	//退場モーション
	BackMotion();

	//基底クラスのUpdate
	Update();
}

void Enemy::Damage(int damage)
{
	//攻撃を受けたときに変色
	enemyColor_ = changeColor_;
	//ダメージ処理
	hp_ -= damage;
	if (hp_ <= 0)
	{
		isDead_ = true;
	}
}

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
		Damage(damage_);
	}
}

void Enemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + (afterInitY_ - (initY_ * MathFunc::easeOutBack(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isInit_ = true;
			initTime_ = 0.0f;
			beforeMovePos_ = worldTransform_.position_;
		}
	}
}

void Enemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ + initY_ * MathFunc::easeInBack(initTime_ / initTimeMax_);
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
}
