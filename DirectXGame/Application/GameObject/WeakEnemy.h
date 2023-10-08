﻿#pragma once
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

	//setter
	//gameScene
	void SetGameScene(GamePlayScene* gameScene) { gameScene_ = gameScene; }

	//phase
	void SetPhase(Phase phase) { phase_ = phase; }

	bool GetIsDead() const { return isDead_; }

	bool GetIsDelete() const { return isDelete_; }

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//移動
	void Move();

	//カーブ
	void RCurve();
	void LCurve();

	//ワールド座標を取得
	Vector3 GetPosition();

private:
	//ゲームシーン
	GamePlayScene* gameScene_ = nullptr;

	//デスフラグ
	//倒されたか
	bool isDead_ = false;
	//デスポーンしたか
	bool isDelete_ = false;
	//弾が当たったか
	bool isHit_ = false;

	// モデル
	Model* wEnemyModel = nullptr;

	//敵の移動パターン
	Phase phase_ = Phase::None;

	//曲がる大きさ
	const float C = 0.5f;
	//初速
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//落下時間
	float flame = 0.0f;
	//体力
	int hp_ = 1;
};