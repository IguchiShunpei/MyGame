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
public:
	//デストラクタ
	~Enemy();
	//初期化
	void EnemyInitialize();

	//更新
	void Update();

	//当たり判定更新
	void ColliderUpdate();

	//gameSceneのsetter
	void SetGameScene(GamePlayScene* gameScene) { gameScene_ = gameScene; }

	bool GetIsDead() const { return isDead_; }
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//攻撃
	void Attack();

	//弾を描画
	void BulletDraw(ViewProjection* viewProjection_);

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	//ゲームシーン
	GamePlayScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	// モデル
	Model* enemyModel = nullptr;

	//弾
	EnemyBullet* enemyBullet;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//打ち出すまでの時間
	float dalayTimer = 5.0f;

	//半径
	float radius = 1.0f;
};
