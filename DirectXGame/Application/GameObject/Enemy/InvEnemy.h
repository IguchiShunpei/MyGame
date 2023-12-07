#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include "MathFunc.h"

//GameSceneの前方宣言
class GameScene;

class InvEnemy : public Object3d
{
public:
	//デストラクタ
	~InvEnemy();
	//初期化
	void InvEnemyInitialize();

	//更新
	void Update(Vector3 playerPos_);

	//当たり判定更新
	void ColliderUpdate();

	//getter
	//IsDelete
	bool GetIsDelete() const { return isDelete_; }
	//isHit
	bool GetIsHit() const { return isHit_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;
	
	//移動
	void Move();

	//回転
	void Turn(Vector3 playerPos_);

	//登場モーション
	void InitMotion();

	//退場モーション
	void BackMotion();

	//Setter
	//gameScene
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	//beforeY
	void SetBeforeY(float beforeY) { beforeY_ = beforeY; }

	//ワールド座標を取得
	Vector3 GetPosition();

private:
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	//デスポーンしたか
	bool isDelete_ = false;

	//登場したか
	bool isInit_ = false;

	//登場時間
	float initTime_ = 0.0f;

	//退場したか
	bool isBack_ = false;

	//初期Y座標
	float beforeY_;

	// モデル
	Model* enemyModel = nullptr;
	//待機時間
	int waitTimer = 0;
	//回転時間
	float turnTimer_ = 0.0f;
	//当たったか
	bool isHit_ = false;
	//発射したか
	bool isStart_ = false;
	//予備動作をしたか
	bool isTurn_ = false;

	Vector3 velocity_;
};
