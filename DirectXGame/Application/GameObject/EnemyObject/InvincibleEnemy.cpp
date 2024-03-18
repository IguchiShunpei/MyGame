#include "InvincibleEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
InvincibleEnemy::~InvincibleEnemy()
{
}

//初期化
void InvincibleEnemy::InvincibleEnemyInitialize()
{
	//敵汎用初期化
	EnemyInitialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("InvincibleEnemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel.get());
	isDelete_ = false;
	isHit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 180.0f;
	velocity_ = { 0.0f,0.0f,0.0f };
}

void InvincibleEnemy::InvincibleEnemyUpdate(Vector3 playerPos_)
{
	//登場
	InitMotion();
	if (isInit_ == true)
	{
		isHit_ = false;
		if (isTurn_ == false && isStart_ == false)
		{
			waitTimer++;
			if (waitTimer >= 120)
			{
				isTurn_ = true;
			}
		}
		//予備動作
		Turn(playerPos_);
		//移動
		Move();
	}
	//退場
	BackMotion();

	EnemyUpdate();
}

void InvincibleEnemy::Move()
{
	if (isStart_ == true)
	{
		worldTransform_.position_ -= velocity_;
		if (worldTransform_.position_.z <= -20.0f)
		{
			isBack_ = true;
		}
	}
}

void InvincibleEnemy::Turn(Vector3 playerPos_)
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

			//敵の速度
			const float kEnemySpeed = 5.0f;

			//プレイヤーのワールド座標の取得
			Vector3 playerPosition;
			playerPosition = playerPos_;

			//自キャラの座標をコピー
			Vector3 enemyPosition = GetPosition();

			//差分ベクトルを求める
			velocity_ = enemyPosition - playerPosition;

			//長さを求める
			velocity_.length();
			//正規化
			velocity_.normalize();

			//ベクトルの長さを,速さに合わせる
			velocity_ *= kEnemySpeed;//これが速度になる
		}
	}
}

void InvincibleEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + (afterInitY_ - (initY_ * MathFunc::easeOutBack(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isInit_ = true;
			initTime_ = 0.0f;
		}
	}
}

void InvincibleEnemy::BackMotion()
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