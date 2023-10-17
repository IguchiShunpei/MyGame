#include "Meteor.h"
#include "GamePLayScene.h"

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
	isUp_ = true;
}

void Meteor::MeteorUpdate()
{
	//移動
	Move();
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
