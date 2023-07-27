#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//GameSceneの前方宣言
class GamePlayScene;

class Enemy : public Object3d
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
	~Enemy();
	//初期化
	void EnemyInitialize();

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

	//攻撃
	void Attack();
	
	//移動
	void Move();

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

	//phaseNumのsetter
	void SetPhaseNum(int phaseNum) { this->phaseNum_ = phaseNum; }

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

	// モデル
	Model* enemyModel = nullptr;

	//弾
	EnemyBullet* enemyBullet;

	//弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//弾を打ち出すまでの時間
	float dalayTimer = 5.0f;

	//行動パターン
	int phaseNum_ = 0;

	//接近速度
	Vector3 approach_ = { 0.0f,0.0f,-0.3f };

	//離脱速度
	Vector3 leave_ = { 0.0f,0.0f,1.0f };
	
	//敵の移動パターン
	Phase phase_ = Phase::None;

	//曲がる大きさ
	const float C = 0.5f;
	//初速
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//落下時間
	float flame = 0.0f;
};
