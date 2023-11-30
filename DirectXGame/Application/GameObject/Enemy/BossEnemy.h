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
		Wait,
		Move,
		Turn,
		Bullet,
		Leave,   //離脱
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

	//行動パターン遷移
	void PhaseChange(Vector3 playerPos);

	//待機
	void Wait();

	//待機モーション
	void WaitMotion();

	//移動
	void Move();

	//初期位置に戻る
	void Leave();

	//攻撃
	//弾
	void BulletAttack();
	//回転攻撃(無敵)
	void TurnAttack(Vector3 pos);

	//登場モーション
	void InitMotion();

	void Damage();

	//死亡するまでのタイマー処理
	void ActiveDeathTimer();

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
	//フラグ
	//死亡時間が動いたか
	bool isDeathTimer_ = false;
	//倒されたか
	bool isDead_ = false;
	//弾が当たったか
	bool isHit_ = false;
	//登場したか
	bool isInit_ = false;

	//登場時間
	float initTime_;

	//待機モーションの変数
	//待機時間
	float waitTimer_;
	//待機最高時間
	float waitTimerMax_;
	//待機モーション時間
	float waitMotionTimer_;
	//待機モーション最高時間
	float waitMotionTimerMax_;
	//前のy座標
	float beforeY_;
	//浮上モーションの時間
	float levTime_;
	//浮上する向き
	int levDirection_;
	//範囲
	float levRange_;
	//上昇しているか
	bool isUp_;

	//離脱モーションの変数
	//初期位置
	Vector3 normalPos_;
	//離脱時間
	float leaveTimer_;
	//離脱最高時間
	float leaveTimerMax_;
	//離脱前の位置
	Vector3 beforeLeavePos_;

	//回転攻撃の変数
	//フラグ(無敵になるため)
	bool isTurn_;
	//接近フラグ
	bool isApproach_;
	//回転速度
	float turnSpeed_;
	//回転最高速度
	float turnSpeedMax_;
	//予備動作時間
	float turnTimer_;
	//予備動作最高時間
	float turnTimerMax_;
	//移動時間
	float turnAttackTimer_;
	//移動最高時間
	float turnAttackTimerMax_;
	//自機の位置
	Vector3 playerPos_;
	//移動する量
	Vector3 turnAttackPos_;
	//離脱時に回転を戻す時のY回転量を保存
	float backRotaY_;

	//弾攻撃の変数
	//弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//弾を打ち出すまでの時間
	float dalayTimer_ = 15.0f;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//色
	Vector3 bossColor_ = { 1.0f,1.0f,1.0f };

	// モデル
	Model* enemyModel = nullptr;

	//死亡するまでの時間
	int deathTimer_;
	
	//敵の移動パターン
	Phase phase_;

	//体力
	int hp_;

	//alpha
	float alpha_ = 1.0f;
};
