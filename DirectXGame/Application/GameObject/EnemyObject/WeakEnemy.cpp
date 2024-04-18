/**
* @file WeakEnemy.cpp
* @brief 攻撃せずに移動するだけの敵クラス
* @author イグチ_シュンペイ
*/

#include "WeakEnemy.h"
#include "SphereCollider.h"
#include "GamePLayScene.h"
#include "string.h"

//デストラクタ
WeakEnemy::~WeakEnemy()
{

}

//初期化
void WeakEnemy::WEnemyInitialize()
{
	//汎用初期化
	EnemyInitialize();
	// OBJからモデルデータを読み込む
	wEnemyModel = Model::LoadFromOBJ("WeakEnemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(wEnemyModel.get());

	//体力
	hp_ = 1;

	//スコア
	score_ = 200;

	//登場移動量
	initY_ = 60.0f;
	afterInitY_ = -60.0f;
	//回転につかう変数の初期化
	rota_ = 5.0f;
}

void WeakEnemy::WEnemyUpdate()
{
	//登場したら
	if (isInit_ == true)
	{
		//移動
		Move();
	}
	//登場
	InitMotion();
	//退場
	BackMotion();

	//敵汎用更新
	EnemyUpdate();
}

void WeakEnemy::Move()
{
	switch (phase_)
	{
	case Phase::None:
		break;
	case Phase::R:
		worldTransform_.rotation_.y -= rota_;
		MathFunc::CurveProjection(worldTransform_, startSpeedBeside, C, flame);
		break;
	case Phase::L:   //カーブフェーズ
		worldTransform_.rotation_.y += rota_;
		MathFunc::CurveProjection(worldTransform_, { -startSpeedBeside.x,startSpeedBeside.y,startSpeedBeside.z }, -C, flame);
		break;
	case Phase::U:   //カーブフェーズ
		worldTransform_.rotation_.x += rota_;
		MathFunc::HorizontalProjection(worldTransform_, { startSpeedVertical.x,-startSpeedVertical.y,startSpeedVertical.z }, -C, flame);
		break;
	case Phase::D:   //カーブフェーズ
		worldTransform_.rotation_.x -= rota_;
		MathFunc::HorizontalProjection(worldTransform_, startSpeedVertical, C, flame);
		break;
	}
}

void WeakEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + (afterInitY_ + (initY_ * MathFunc::easeOutSine(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isInit_ = true;
			initTime_ = 0.0f;
		}
	}
}

void WeakEnemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ - initY_ * MathFunc::easeInSine(initTime_ / initTimeMax_);
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
}