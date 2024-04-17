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
	//オブジェクト初期化
	EnemyInitialize();
	if (isBack_ == false)
	{
		// OBJからモデルデータを読み込む
		meteorModel_ = Model::LoadFromOBJ("meteor");
	}
	else
	{
		meteorModel_ = Model::LoadFromOBJ("backMeteor");
	}
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(meteorModel_.get());

	velocity_ = { 0.0f,0.0f,0.3f };
	frontZ_ = -200.0f;
	backZ_ = 600.0f;

	rotaSpeed_ = 1.0f;
	worldTransform_.scale_ = Vector3(2.0f, 2.0f, 2.0f);

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	//ランダムで回転方向を割り当てる
	std::uniform_int_distribution<> rotaDirection(0, 5);
	rotaDirection_ = rotaDirection(engine);

	hp_ = 1;

	score_ = 100;

	isHit_ = false;
	isHitEnd_ = true;
	isInit_ = false;

	alpha_ = 0.0;
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
	}

	//更新
	EnemyUpdate();
}

void Meteor::Move()
{
	//移動
	worldTransform_.position_ -= velocity_;
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

void Meteor::SetSpeed(float min, float max)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>speed(min, max);

	velocity_.z = speed(engine);
}

