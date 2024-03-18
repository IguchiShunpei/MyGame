/**
* @file WeakEnemy.cpp
* @brief 攻撃せずに移動するだけの敵クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Enemy.h"
#include <list>

class WeakEnemy : public Enemy
{
public:
	//移動パターン
	enum Phase
	{
		None,
		R,   //右カーブ
		L,   //左カーブ 
		U,   //上カーブ
		D,   //下カーブ
	};

public:
	//デストラクタ
	~WeakEnemy();
	//初期化
	void WEnemyInitialize();

	//更新
	void WEnemyUpdate();

	//登場
	void InitMotion();

	//退場
	void BackMotion();

	//setter

	//phase
	void SetPhase(Phase phase) { phase_ = phase; }

	//移動
	void Move();

private:
	// モデル
	std::unique_ptr <Model> wEnemyModel;

	//敵の移動パターン
	Phase phase_ = Phase::None;

	//曲がる大きさ
	const float C = 0.5f;
	//初速
	Vector3 startSpeedBeside = { -0.5f,0.0f,-0.5f };
	Vector3 startSpeedVertical = { 0.0f,0.5f,-0.5f };
	//落下時間
	float flame = 0.0f;

	float rota_;
};
