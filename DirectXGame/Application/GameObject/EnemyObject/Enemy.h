/**
* @file Enemy.cpp
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

//GameSceneの前方宣言
class GameScene;

class Enemy : public Object3d
{
	//部位
	enum class Part
	{
		Head,
		Leg
	};

public:
	//移動パターン
	enum class Phase
	{
		None,
		Approach,//接近
		Leave,   //離脱
		Curve,   //カーブ
		ReCurve, //逆方向カーブ
	};

public:
	//デストラクタ
	~Enemy();
	//初期化
	void EnemyInitialize();

	//更新
	void Update(Vector3 playerPos_);

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
	Vector3 GetColor() const { return originalColor_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

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
	void SetPhase(Phase phase) { phase_ = phase; }
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
	//退場までの時間
	float deleteTimer_;
	//攻撃したか
	bool isAttack_ = false;

	// モデル
	std::unique_ptr <Model> enemyModel_01;
	std::unique_ptr <Model> enemyModel_02;
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

	//接近速度
	Vector3 approach_ = { 0.0f,0.0f,-0.3f };

	//離脱速度
	Vector3 leave_ = { 0.0f,0.0f,1.0f };
	
	//敵の移動パターン
	Phase phase_ = Phase::None;

	//体力
	int hp_;

	//初期Y座標
	float beforeY_;

	//登場時の移動量
	float moveY_;
	float afterMoveY_;

	//弾の数
	int bulletNum_;

	//弾の最大数
	int bulletMax_;

	//ダメージ
	int damage_;
};
