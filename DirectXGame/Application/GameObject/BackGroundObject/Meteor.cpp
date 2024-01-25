/**
* @file Meteor.cpp
* @brief 隕石の処理クラス
* @author イグチ_シュンペイ
*/

#include "Meteor.h"
#include "GamePlayScene.h"
#include "SphereCollider.h"
#include "string.h"

void Meteor::MeteorInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	meteorModel_ = Model::LoadFromOBJ("meteor");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(meteorModel_);

	speed_ = 0.3f;
	frontZ_ = -200.0f;
	backZ_ = 600.0f;

	rotaSpeed_ = 1.0f;
	worldTransform_.scale_ = Vector3(2.0f, 2.0f,2.0f);

	hp_ = 3;

	hitColor_ = {3.0f,3.0f,3.0f};

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	//ランダムで回転方向を割り当てる
	std::uniform_int_distribution<> rotaDirection(0, 5);
	rotaDirection_ = rotaDirection(engine);

	isHit_ = false;
	isHitEnd_ = true;
	isInit_ = false;

	alpha_ = 0.0;
	alphaMax_ = 1.0f;
};

void Meteor::MeteorUpdate()
{
	if (alpha_ < alphaMax_)
	{
		alpha_ += 0.01f;
	}
	else
	{
		isInit_ = true;
		alpha_ = alphaMax_;
	}


	if (isDead_ == false)
	{
		color_ = { 1.0f,1.0f,1.0f };
		//移動
		Move();
		//回転
		Rotate();
		//当たり判定更新
		ColliderUpdate();
		if (isBack_ == false)
		{
			//ダメージ処理
			Damage();
		}
	}
	//更新
	Update();
}

void Meteor::Move()
{
	//移動
	worldTransform_.position_.z -= speed_;
	//カメラよりも奥に進んだら削除
	if (worldTransform_.position_.z <= frontZ_)
	{
		//背景フラグで処理を変える
		if (isBack_ == false)
		{
			//削除
			isDelete_ = true;
		}
		else
		{
			//画面奥まで戻る
			worldTransform_.position_.z = backZ_;
			alpha_ = 0.0f;
		}
	}
}

void Meteor::Rotate()
{
	//割り当てられた回転の向きを実行
	switch (rotaDirection_)
	{
	case 0:
		worldTransform_.rotation_.x += rotaSpeed_;
		break;
	case 1:
		worldTransform_.rotation_.x -= rotaSpeed_;
		break;
	case 2:
		worldTransform_.rotation_.y += rotaSpeed_;
		break;
	case 3:
		worldTransform_.rotation_.y -= rotaSpeed_;
		break;
	case 4:
		worldTransform_.rotation_.z += rotaSpeed_;
		break;
	case 5:
		worldTransform_.rotation_.z -= rotaSpeed_;
		break;
	}
}

void Meteor::Damage()
{
	if (isHit_ == true)
	{
		hp_--;
		color_ = hitColor_;
		if (hp_ <= 0)
		{
			isDead_ = true;
		}
	}
	isHit_ = false;
}

void Meteor::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Meteor::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
	}
}

void Meteor::SetSpeed(float min,float max)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>speed(min, max);

	speed_ = speed(engine);
}

