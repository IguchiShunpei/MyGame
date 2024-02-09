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
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy03");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel.get());
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 60.0f;
	velocity_ = { 0.0f,0.0f,0.0f };
}

void InvincibleEnemy::Update(Vector3 playerPos_)
{
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
		Turn(playerPos_);
		Move();
	}
	BackMotion();

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void InvincibleEnemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void InvincibleEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	if (isInit_ == true)
	{
		//相手がplayerBullet
		if (strcmp(toCollisionName, str1) == 0)
		{
			isHit_ = true;
		}
	}
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
			const float kEnemySpeed = 3.0f;

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
		worldTransform_.position_.y = beforeY_ + 60.0f * MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_--;
		if (initTime_ <= 0.0f)
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
		worldTransform_.position_.y = beforeY_ + 120.0f * MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_++;
		if (initTime_ >= 60.0f)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
}


Vector3 InvincibleEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
