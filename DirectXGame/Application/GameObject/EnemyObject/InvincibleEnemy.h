/**
* @file InvincibleEnemy.cpp
* @brief 攻撃では倒せない敵の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Enemy.h"
#include <list>

//GameSceneの前方宣言
class GameScene;

class InvincibleEnemy : public Enemy
{
public:
	//デストラクタ
	~InvincibleEnemy();
	//初期化
	void InvincibleEnemyInitialize();

	//更新
	void InvincibleEnemyUpdate(Vector3 playerPos_);
	
	//移動
	void Move();

	//回転
	void Turn(Vector3 playerPos_);

	//登場モーション
	void InitMotion();

	//退場モーション
	void BackMotion();

private:

	// モデル
	std::unique_ptr <Model> enemyModel;
	//待機時間
	int waitTimer = 0;
	//回転時間
	float turnTimer_ = 0.0f;
	//当たったか
	bool isHit_ = false;
	//発射したか
	bool isStart_ = false;
	//予備動作をしたか
	bool isTurn_ = false;
	//移動量
	Vector3 velocity_;
};
