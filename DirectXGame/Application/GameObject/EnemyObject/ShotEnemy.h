/**
* @file ShotEnemy.cpp
* @brief 弾を発射する敵の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Enemy.h"
#include "EnemyBullet.h"
#include <list>

class ShotEnemy : public Enemy
{
public:
	//移動パターン
	enum MoveNum
	{
		Up,
		Down,
		Right,
		Left,
	};

public:
	//デストラクタ
	~ShotEnemy();
	//初期化
	void ShotEnemyInitialize();

	//更新
	void ShotEnemyUpdate(Vector3 playerPos);


	void Move();

	//攻撃
	void Attack(Vector3 playerPos_);

	//登場モーション
	void InitMotion();

	//退場モーション
	void BackMotion();

	//弾を描画
	void BulletDraw(ViewProjection* viewProjection_);

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//Setter
	//move
	void SetMove(MoveNum moveNum) { moveNum_ = moveNum; }
private:
	//攻撃したか
	bool isAttack_ = false;

	// モデル
	std::unique_ptr <Model> enemyModel_01;
	//弾
	std::unique_ptr <EnemyBullet> enemyBullet_;

	//弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//弾の速度
	float kBulletSpeed_;
	//移動量
	Vector3 velocity_;

	//弾を打ち出すまでの時間
	float dalayTimer_;
	//最大
	float delayTImerMax_;

	//弾のcollider
	Vector3 bulletColliderPos_;
	float bulletColliderRadius_;
	
	//敵の移動パターン
	MoveNum moveNum_;

	//移動量
	float moveLen_;

	//弾の数
	int bulletNum_;

	//弾の最大数
	int bulletMax_;

	//登場後の座標
	Vector3 beforeMovePos_;

	//移動時間
	float moveTime_;
	float moveTimeMax_;
};
