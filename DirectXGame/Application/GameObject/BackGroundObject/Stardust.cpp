#include "Stardust.h"
#include "GamePlayScene.h"

void Stardust::StardustInitialize()
{
	//初期化
	Initialize();
	// 3Dオブジェクト生成
	Create();

	speed_ = 0.2f;
	frontX_ = -20.0f;
	backZ_ = 70.0f;
	levRange_ = 1.0f;

	rotaSpeed_ = 2.0f;

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
	std::uniform_int_distribution<> stardustSize(0, 1);
	stardustSize_ = stardustSize(engine);

	isUp_ = true;
}

void Stardust::StardustUpdate()
{
	//移動
	Move();
	//回転
	Rotate();
	//浮上モーション
	Levitate();
	//更新
	Update();
}

void Stardust::SetSize()
{
	//割り当てられたサイズを実行
	switch (stardustSize_)
	{
	case 0:
		SetScale({ 0.06f,0.06f,0.06f });
		break;
	case 1:
		SetScale({ 0.04f,0.04f,0.04f });
		break;
	}
}

void Stardust::Move()
{
	//移動
	worldTransform_.position_.z -= speed_;
	//カメラの裏まで進んだら画面奥まで戻る
	if (worldTransform_.position_.z <= frontX_)
	{
		worldTransform_.position_.z = backZ_;
	}
}

void Stardust::Rotate()
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

void Stardust::Levitate()
{
	if (levDirection_ == 0)
	{
		//上昇
		if (isUp_ == true)
		{
			stardustY_ = levRange_ * MathFunc::easeOutSine(levTime_ / 60.0f);
			worldTransform_.position_.y = beforeY_ + stardustY_;
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
			stardustY_ = levRange_ * -MathFunc::easeOutSine(levTime_ / 60.0f);
			worldTransform_.position_.y = beforeY_ + stardustY_;
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
			stardustY_ = levRange_ * MathFunc::easeOutSine(levTime_ / 60.0f);
			worldTransform_.position_.y = beforeY_ + stardustY_;
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
			stardustY_ = levRange_ * -MathFunc::easeOutSine(levTime_ / 60.0f);
			worldTransform_.position_.y = beforeY_ + stardustY_;
			levTime_++;
			if (levTime_ >= 120)
			{
				isUp_ = false;
				levTime_ = 0;
			}
		}
	}

}