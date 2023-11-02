#include "Meteor.h"
#include "GamePlayScene.h"

void Meteor::MeteorInitialize()
{
	//初期化
	Initialize();
	// 3Dオブジェクト生成
	Create();

	speed_ = 0.05f;
	frontX_ = -20.0f;
	backZ_ = 70.0f;
	levRange_ = 1.0f;

	rotaSpeed_ = 2.0f;
	size_ = Vector3(0.0f, 0.0f, 0.0f);

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	//ランダムで回転方向を割り当てる
	std::uniform_int_distribution<> rotaDirection(0, 5);
	rotaDirection_ = rotaDirection(engine);
	//ランダムで上下を割り当てる
	std::uniform_int_distribution<> levDirection(0, 1);
	levDirection_ = levDirection(engine);
	//ランダムでサイズを割り当てる
	std::uniform_int_distribution<> meteorSize(0, 3);
	meteorSize_ = meteorSize(engine);

	SetSize();

	isUp_ = true;
}

void Meteor::MeteorUpdate()
{
	//スケール
	Scale();
	//移動
	Move();
	//回転
	Rotate();
	//浮上モーション
	Levitate();
	//更新
	Update();
}

void Meteor::Scale()
{
	if (worldTransform_.scale_.x < size_.x)
	{
		worldTransform_.scale_.x += 0.005f;
		worldTransform_.scale_.y += 0.005f;
		worldTransform_.scale_.z += 0.005f;
	}
	else
	{
		worldTransform_.scale_ = size_;
	}
}

void Meteor::SetSize()
{
	//割り当てられたサイズを実行
	switch (meteorSize_)
	{
	case 0:
		size_ = {0.25f,0.25f,0.25f};
		break;
	case 1:
		size_ = { 0.5f,0.5f,0.5f };
		break;
	case 2:
		size_ = { 1.0f,1.0f,1.0f };
		break;
	case 3:
		size_ = { 1.5f,1.5f,1.5f };
		break;
	}
}

void Meteor::Move()
{
	//移動
	worldTransform_.position_.z -= speed_;
	//カメラの裏まで進んだら画面奥まで戻る
	if (worldTransform_.position_.z <= frontX_)
	{
		worldTransform_.position_.z = backZ_;
		worldTransform_.scale_.x = 0.0f;
		worldTransform_.scale_.y = 0.0f;
		worldTransform_.scale_.z = 0.0f;
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

void Meteor::Levitate()
{
	if (levDirection_ == 0)
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
	else
	{
		//上昇
		if (isUp_ == false)
		{
			meteorY_ = levRange_ * MathFunc::easeOutSine(levTime_ / 60.0f);
			worldTransform_.position_.y = beforeY_ + meteorY_;
			levTime_++;
			if (levTime_ >= 120)
			{
				isUp_ = true;
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
				isUp_ = false;
				levTime_ = 0;
			}
		}
	}
	
}
