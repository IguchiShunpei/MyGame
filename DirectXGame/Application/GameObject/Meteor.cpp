#include "Meteor.h"
#include "GamePLayScene.h"
#include <fstream>
#include <string.h>
#include <math.h>

void Meteor::MeteorInitialize()
{
	//初期化
	Initialize();
	// 3Dオブジェクト生成
	Create();

	speed_ = 0.05f;
	frontNum_ = -20.0f;
	backNum_ = 70.0f;
	levRange_ = 1.0f;

	rotaSpeed_ = 1.0f;

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::uniform_int_distribution<> dist(0, 5);
	rotaNum_ = dist(engine);

	isUp_ = true;
}

void Meteor::MeteorUpdate()
{
	//移動
	Move();
	//回転
	Rotate();
	//浮上モーション
	Levitation();
	//更新
	Update();
}

void Meteor::Move()
{
	worldTransform_.position_.z -= speed_;
	if (worldTransform_.position_.z <= frontNum_)
	{
		worldTransform_.position_.z = backNum_;
	}
}

void Meteor::Rotate()
{
	//割り当てられた回転の向きを実行
	switch (rotaNum_)
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

void Meteor::Levitation()
{
	//上昇
	if (isUp_ == true)
	{
		meteorY_ = levRange_ * MathFunc::easeOutSine(levTime_ / 60.0f);
		worldTransform_.position_.y = beforeY_ + meteorY_;
		levTime_++;
		if (levTime_ >= 120)
		{
			isUp_ = false;
			levTime_ = 0;
		}
	}
	//下降
	else
	{
		meteorY_ = levRange_ * -MathFunc::easeOutSine(levTime_ / 60.0f);
		worldTransform_.position_.y = beforeY_ + meteorY_;
		levTime_++;
		if (levTime_ >= 120)
		{
			isUp_ = true;
			levTime_ = 0;
		}
	}
}
