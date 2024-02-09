/**
* @file WeakEnemy.cpp
* @brief 攻撃せずに移動するだけの敵クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include <list>

//GameSceneの前方宣言
class GamePlayScene;

class WeakEnemy : public Object3d
{
public:
	//移動パターン
	enum Phase
	{
		None,
		R,   //右カーブ
		L,   //左カーブ
	};

public:
	//デストラクタ
	~WeakEnemy();
	//初期化
	void WEnemyInitialize();

	//更新
	void Update();

	//当たり判定更新
	void ColliderUpdate();

	//ダメージ
	void Damage(int damage);

	//setter
	//beforeY
	void SetBeforeY(float beforeY) { beforeY_ = beforeY; }

	//phase
	void SetPhase(Phase phase) { phase_ = phase; }

	//damage
	void SetDamage(int damage) { damage_ = damage; }

	bool GetIsDead() const { return isDead_; }

	bool GetIsDelete() const { return isDelete_; }

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//color
	Vector3 GetColor() const { return enemyColor_; }

	//移動
	void Move();

	//登場モーション
	void InitMotion();

	//退場モーション
	void BackMotion();

	//カーブ
	void RCurve();
	void LCurve();

	//ワールド座標を取得
	Vector3 GetPosition();

private:

	//フラグ
	//倒されたか
	bool isDead_ = false;
	//デスポーンしたか
	bool isDelete_ = false;
	//登場したか
	bool isInit_ = false;
	//退場したか
	bool isBack_ = false;
	//登場時間
	float initTime_ = 0.0f;

	//初期Y座標
	float beforeY_;

	// モデル
	std::unique_ptr <Model> wEnemyModel;

	//敵の移動パターン
	Phase phase_ = Phase::None;

	//曲がる大きさ
	const float C = 0.5f;
	//初速
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//落下時間
	float flame = 0.0f;
	//体力
	int hp_ = 2;
	//ダメージ
	int damage_;

	Vector3 enemyColor_;
};
