/**
* @file Enemy.cpp
* @brief 汎用的な敵の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>


class Enemy : public Object3d
{
public:
	//デストラクタ
	~Enemy();
	//初期化
	void EnemyInitialize();

	//更新
	void EnemyUpdate();

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

	//登場モーション
	void InitMotion();

	//退場モーション
	void BackMotion();

	//Setter
	//HP
	void SetHp(int hp) { this->hp_ = hp; }
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

	//体力
	int hp_;

	//初期Y座標
	float beforeY_;

	//登場時の移動量
	float initY_;
	float afterInitY_;

	//ダメージ
	int damage_;

	//登場後の座標
	Vector3 beforeMovePos_;
};
