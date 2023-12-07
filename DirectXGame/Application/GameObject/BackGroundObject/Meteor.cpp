#include "Meteor.h"
#include "GamePlayScene.h"

void Meteor::MeteorInitialize()
{
	//初期化
	Initialize();
	// 3Dオブジェクト生成
	Create();

	speed_ = 0.2f;
	frontZ_ = -100.0f;

	rotaSpeed_ = 1.0f;
	size_ = Vector3(5.0f, 5.0f, 5.0f);

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	//ランダムで回転方向を割り当てる
	std::uniform_int_distribution<> isRotation(0, 1);
	isRota_ = isRotation(engine);

}

void Meteor::MeteorUpdate()
{
	//移動
	Move();
	//回転
	Rotate();
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
		isDelete_ = true;
	}
}

void Meteor::Rotate()
{
	//基準のサイズよりも小さい物は回転させる
	if (isRota_ == 1)
	{
		worldTransform_.rotation_.x -= rotaSpeed_;
	}
}

