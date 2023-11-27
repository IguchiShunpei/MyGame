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

	//弾更新
	void BulletUpdate();

	//当たり判定更新
	void ColliderUpdate();

	//getter
	//IsDeathTimer
	bool GetIsDeathTimer() const { return isDeathTimer_; }
	//IsDead
	bool GetIsDead() const { return isDead_; }
	//IsHit
	bool GetIsHit() const { return isHit_; }
	//IsInit
	bool GetIsInit() const { return isInit_; }
	//HP
	int GetHP() const { return hp_; }
	//DeathTimer
	int GetDeathTimer() const { return deathTimer_; }
	//Color
	Vector3 GetColor() const { return bossColor_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//移動
	void Move();

	//攻撃
	void Attack();

	//登場モーション
	void InitMotion();

	//死亡するまでのタイマー処理
	void ActiveDeathTimer();

	//接近
	void Approach();
	//離脱
	void Leave();
	//カーブ
	void Curve();
	void ReCurve();

	//弾を描画
	void BulletDraw(ViewProjection* viewProjection_);

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

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
	//beforeY
	void SetBeforeY(float beforeY) { beforeY_ = beforeY; }

	//ワールド座標を取得
	Vector3 GetPosition();

private:
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//色
	Vector3 bossColor_ = { 1.0f,1.0f,1.0f };

	//デスフラグ
	bool isDeathTimer_ = false;
	//倒されたか
	bool isDead_ = false;
	//弾が当たったか
	bool isHit_ = false;
	//登場したか
	bool isInit_ = false;

	//登場時間
	float initTime_ = 0.0f;

	//初期Y座標
	float beforeY_;

	// モデル
	Model* enemyModel = nullptr;

	//弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

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
