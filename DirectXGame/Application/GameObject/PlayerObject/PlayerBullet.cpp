/**
* @file PlayerBullet.cpp
* @brief 自機の弾の処理クラス
* @author イグチ_シュンペイ
*/

#include "PlayerBullet.h"
#include "SphereCollider.h"

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::BulletInitialize(const Vector3& position, Vector3 velocity, int bulletLevel)
{
	Initialize();
	// OBJからモデルデータを読み込む
	playerBulletModel_ = Model::LoadFromOBJ("PlayerBullet");
	// オブジェクトにモデルをひも付ける
	SetModel(playerBulletModel_.get());
	alpha_ = 1.0f;

	// 3Dオブジェクト生成
	Create();

	velocity_ = velocity;

	SetScale(Vector3(1.0f, 1.0f, 10.0f));
	bulletColor_ = { 1.0f,1.0f,1.0f };
	if (bulletLevel == 2)
	{
		bulletColor_ = { 1.0f,0.0f,0.0f };
	}
	//引数で受け取った初期座標をセット
	worldTransform_.position_ = position;
	//引数で受け取った速度をメンバ変数に代入
	isDelete_ = false;
	scaleNum_ = { 0.02f,0.02f,0.02f };
	rotaNum_ = 20.0f;
	deleteZ_ = 150.0f;
}

void PlayerBullet::LaserInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	playerBulletModel_ = Model::LoadFromOBJ("laser");
	// オブジェクトにモデルをひも付ける
	SetModel(playerBulletModel_.get());

	// 3Dオブジェクト生成
	Create();
	bulletColor_ = { 1.0f,1.0f,1.0f };
	alpha_ = 0.0f;
	alphaMax_ = 0.2f;
	alphaMin_ = 0.0f;
	alphaNum_ = 0.005f;
	laserScale_ = 0.0f;
	laserScaleMin_ = 0.2f;
	laserScaleMax_ = 0.2f;
	isInit_ = false;
	isBack_ = true;
	initTime_ = 0;
	initTimeMax_ = 30;
	laseRotaMax_ = 90.0f;
	isBlink_ = true;
	blinkTime_ = 0.0f;
	blinkTimeMax_ = 180.0f;
}

void PlayerBullet::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void PlayerBullet::BulletUpdate()
{
	//回転
	worldTransform_.rotation_.z += rotaNum_;
	worldTransform_.position_ += velocity_;
	//時間経過で弾が消える
	if (worldTransform_.position_.z >= deleteZ_)
	{
		isDelete_ = true;
	}
	if (worldTransform_.scale_.x >= 0.0f)
	{
		worldTransform_.scale_ -= scaleNum_;

	}
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::LaserUpdate(Vector3 position, Vector3 rotation)
{
	if (isInit_ == false)
	{
		LaserInit(position, rotation);
	}
	else
	{
		//点滅
		Blining();
		//回転と座標の更新
		worldTransform_.position_ = position;
		worldTransform_.rotation_ = rotation;
		//消去
		LaserOut();
		worldTransform_.UpdateMatrix();
	}
}

void PlayerBullet::LaserInit(Vector3 position, Vector3 rotation)
{
	if (isInit_ == false)
	{
		if (initTime_ <= initTimeMax_)
		{
			if (alpha_ <= alphaMax_)
			{
				//alpha値
				alpha_ = alphaMax_ * MathFunc::easeOutSine(initTime_ / initTimeMax_);
			}
			//スケール
			laserScale_ = laserScaleMin_ * MathFunc::easeOutSine(initTime_ / initTimeMax_);

			worldTransform_.position_ = position;
			worldTransform_.rotation_ = rotation;
			worldTransform_.scale_ = { laserScale_,laserScale_ ,laserScale_ };
			worldTransform_.UpdateMatrix();
			initTime_++;
		}
		else
		{
			isInit_ = true;
			isBlink_ = true;
		}
	}
}

void PlayerBullet::LaserOut()
{
	if (isBack_ == false)
	{
		if (initTime_ <= initTimeMax_)
		{
			if (alpha_ >= 0.0f)
			{
				//alpha値
				alpha_ = alphaMax_ - (alphaMax_ * MathFunc::easeOutSine(initTime_ / initTimeMax_));
			}
			//スケール
			laserScale_ = laserScaleMax_
				- (laserScaleMin_ * MathFunc::easeOutSine(initTime_ / initTimeMax_));

			worldTransform_.scale_ = { laserScale_,laserScale_ ,laserScale_ };
			worldTransform_.UpdateMatrix();
			initTime_++;
		}
		else
		{
			isBack_ = true;
			isBlink_ = false;
		}
	}
}

void PlayerBullet::Blining()
{
	if (isBlink_ == true)
	{
		if (isOn_ == true)
		{
			//濃くなる
			if (blinkTime_ <= blinkTimeMax_)
			{
				alpha_ = alphaMax_ * MathFunc::easeInSine(blinkTime_ / blinkTimeMax_);
			}
			else
			{
				blinkTime_ = 0.0f;
				isOn_ = false;
			}
		}
		else
		{
			//薄くなる
			if (blinkTime_ <= blinkTimeMax_)
			{
				alpha_ = alphaMax_ - (alphaMax_ * MathFunc::easeOutSine(blinkTime_ / blinkTimeMax_));
			}
			else
			{
				blinkTime_ = 0.0f;
				isOn_ = true;
			}
		}
	}
	blinkTime_++;
}

void PlayerBullet::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class Enemy";
	const char* str2 = "class WeakEnemy";
	const char* str3 = "class EnemyBullet";
	const char* str4 = "class InvEnemy";
	const char* str5 = "class Meteor";
	const char* str6 = "class BossEnemy";

	//相手がEnemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0 ||
		strcmp(toCollisionName, str3) == 0 || strcmp(toCollisionName, str4) == 0 ||
		strcmp(toCollisionName, str5) == 0 || strcmp(toCollisionName, str6) == 0)
	{
		isDelete_ = true;
	}
}