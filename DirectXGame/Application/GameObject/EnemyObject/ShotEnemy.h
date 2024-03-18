/**
* @file ShotEnemy.cpp
* @brief 弾を発射する敵の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class ShotEnemy : public Object3d
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

	//当たり判定更新
	void ColliderUpdate();

	//ダメージ
	void Damage(int damage);

	//getter
	//IsDead
	bool GetIsDead() const { return isDead_; }
	//IsDelete
	bool GetIsDelete() const { return isDelete_; }
	//IsHit
	bool GetIsHit() const { return isHit_; }
	//HP
	int GetHP() const { return hp_; };
	//color
	Vector3 GetColor() const { return enemyColor_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

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
	//HP
	void SetHp(int hp) { this->hp_ = hp; }
	//phase
	void SetMove(MoveNum moveNum) { moveNum_ = moveNum; }
	//isDead
	void SetIsDead(bool isDead) { this->isDead_ = isDead; }
	//beforeY
	void SetBeforeY(float beforeY) { beforeY_ = beforeY; }
	//damage
	void SetDamage(int damage) { damage_ = damage; }

	//ワールド座標を取得
	Vector3 GetPosition();

private:

	//色
	Vector3 enemyColor_;
	Vector3 originalColor_;
	Vector3 changeColor_;

	//デスフラグ
	//倒されたか
	bool isDead_ = false;
	//デスポーンしたか
	bool isDelete_ = false;
	//弾が当たったか
	bool isHit_ = false;

	//登場したか
	bool isInit_ = false;
	//退場したか
	bool isBack_ = false;
	//登場時間
	float initTime_;
	//登場時間
	float initTimeMax_;
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

	//体力
	int hp_;

	//初期Y座標
	float beforeY_;

	//登場時の移動量
	float initY_;
	float afterInitY_;

	//弾の数
	int bulletNum_;

	//弾の最大数
	int bulletMax_;

	//ダメージ
	int damage_;

	//登場後の座標
	Vector3 beforeMovePos_;

	//移動時間
	float moveTime_;
	float moveTimeMax_;
};
