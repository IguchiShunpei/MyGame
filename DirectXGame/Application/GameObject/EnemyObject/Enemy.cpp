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
	//Object3d初期化
	Initialize();

	//変数初期化
	isDead_ = false;
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 90.0f;
	initY_ = 60.0f;
	afterInitY_ = 60.0f;

	color_ = { 0.0f,0.0f,0.0f };
	originalColor_ = { 1.0f,1.0f,1.0f };
	changeColor_ = { 3.0f,3.0f,3.0f };
}

void Enemy::EnemyUpdate()
{
	isHit_ = false;
	color_ = originalColor_;

	//基底クラスのUpdate
	Update();
}

void Enemy::Damage(int damage)
{
	//攻撃を受けたときに変色
	color_ = changeColor_;
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
