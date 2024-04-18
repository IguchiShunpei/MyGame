#include "Score.h"
/**
* @file Score.cpp
* @brief  スコア処理に関するクラス
* @author イグチ_シュンペイ
*/

Score::~Score()
{

}

void Score::ScoreInitialize()
{
	EnemyInitialize();
	switch (enemyObject_)
	{
	case EnemyObject::NONE:
		break;
	case EnemyObject::METEOR: //隕石の初期化
		scoreEnemyModel_ = Model::LoadFromOBJ("meteor");
		break;
	case EnemyObject::WEAKENEMY: //弱い敵の初期化
		scoreEnemyModel_ = Model::LoadFromOBJ("WeakEnemy");
		break;
	case EnemyObject::SHOTENEMY: //弾を撃つ敵の初期化
		scoreEnemyModel_ = Model::LoadFromOBJ("enemy");
		break;
	}
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(scoreEnemyModel_.get());
	SetPosition({ 10.0f,-2.0f,0.0f });
	SetScale(Vector3(1.0f, 1.0f, 1.0f));
	worldTransform_.UpdateMatrix();

	color_ = { 1.0f,1.0f,1.0f };
	alpha_ = 1.0f;

	scoreTime_ = 0.0f;
	scoreTimeMax_ = 60.0f;

}

void Score::ScoreUpdate()
{
	if (scoreTime_ <= scoreTimeMax_)
	{
		worldTransform_.rotation_.y += 1.0f;
		worldTransform_.position_.y = -2.0f - (2.0f * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_));
		worldTransform_.scale_.x = 1.0f - (1.0f * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_));
		worldTransform_.scale_.y = 1.0f - (1.0f * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_));
		worldTransform_.scale_.z = 1.0f - (1.0f * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_));
		alpha_ = 1.0f - (1.0f * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_));
		worldTransform_.rotation_.y = 360 * MathFunc::easeInSine(scoreTime_ / scoreTimeMax_);

		scoreTime_++;
	}
	else
	{
		alpha_ = 0.0f;
		scoreTime_ = 0.0f;
		isDelete_ = true;
	}
	worldTransform_.UpdateMatrix();
}