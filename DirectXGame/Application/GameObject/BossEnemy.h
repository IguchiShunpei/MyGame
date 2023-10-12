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

class BossEnemy : public Object3d
{
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
	~BossEnemy();
	//初期化
	void BossEnemyInitialize();

	//更新
	void Update();

	//描画
	void BossDraw();

	//当たり判定更新
	void ColliderUpdate();

	//getter
	//IsDeathTimer
	bool GetIsDeathTimer() const { return isDeathTimer_; }
	//IsDead
	bool GetIsDead() const { return isDead_; }
	//IsHit
	bool GetIsHit() const { return isHit_; }
	//HP
	int GetHP() const { return hp_; }
	//DeathTimer
	int GetDeathTimer() const { return deathTimer_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;
	
	//移動
	void Move();

	//死亡するまでのタイマー処理
	void ActiveDeathTimer();

	//接近
	void Approach();
	//離脱
	void Leave();
	//カーブ
	void Curve();
	void ReCurve();

	//Setter
	//HP
	void SetHp(int hp) { this->hp_ = hp; }
	//gameScene
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	//phase
	void SetPhase(Phase phase) { phase_ = phase; }
	//deathTimer
	void SetDeathTimer(int deathTimer) { this->deathTimer_ = deathTimer; }
	//alpha
	void SetAlpha(float) {}

	//ワールド座標を取得
	Vector3 GetPosition();

private:
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDeathTimer_ = false;
	//倒されたか
	bool isDead_ = false;
	//弾が当たったか
	bool isHit_ = false;

	// モデル
	Model* enemyModel = nullptr;

	//弾を打ち出すまでの時間
	float dalayTimer_ = 15.0f;

	//死亡するまでの時間
	int deathTimer_ = 180;

	//接近速度
	Vector3 approach_ = { 0.0f,0.0f,-0.3f };

	//離脱速度
	Vector3 leave_ = { 0.0f,0.0f,1.0f };
	
	//敵の移動パターン
	Phase phase_ = Phase::None;

	//体力
	int hp_;

	//曲がる大きさ
	const float C = 0.5f;
	//初速
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//落下時間
	float flame = 0.0f;
	//alpha
	float alpha_ = 1.0f;
};
