/**
* @file Score.cpp
* @brief  スコア処理に関するクラス
* @author イグチ_シュンペイ
*/

#include "Enemy.h"
#include "MathFunc.h"
#include <list>

#pragma once

class Score : public Enemy
{
public:
	//オブジェクト
	enum EnemyObject
	{
		NONE,
		METEOR,
		WEAKENEMY,
		SHOTENEMY
	};
public:
	~Score();
	//初期化
	void ScoreInitialize();
	//更新
	void ScoreUpdate();

	//EnemyObject
	void SetEnemyObject(EnemyObject enemyObject) { enemyObject_ = enemyObject; }

private:
	//スコア表示時間
	float scoreTime_;
	float scoreTimeMax_;

	// モデル
	std::unique_ptr <Model> scoreEnemyModel_;

	//オブジェクト番号
	EnemyObject enemyObject_ = EnemyObject::NONE;
};