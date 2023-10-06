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
	void Update();

	//当たり判定更新
	void ColliderUpdate();

	//getter
	//IsDelete
	bool GetIsDelete() const { return isDelete_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;
	
	//移動
	void Move();

	//回転
	void Turn();

	//Setter
	//gameScene
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	//ワールド座標を取得
	Vector3 GetPosition();

private:
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	//デスポーンしたか
	bool isDelete_ = false;

	// モデル
	Model* enemyModel = nullptr;
	
	int waitTimer = 0;
	float turnTimer_ = 0.0f;

	bool isHit_ = false;

	bool isStart_ = false;

	bool isTurn_ = false;
};
