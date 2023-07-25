#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "MathFunc.h"
#include <memory>
#include <list>

class Player : public Object3d
{
public:
	//デストラクタ
	~Player();
	//初期化
	void PlayerInitialize();

	//更新
	void Update();

	//当たり判定更新
	void ColliderUpdate();

	//登場モーション
	void IntitMotion();

	//移動
	void Move();

	//傾き
	void Rotate();

	//攻撃
	void Attack();

	//弾切り替え
	void ChangeBullet();

	//弾描画
	void BulletDraw(ViewProjection* viewProjection_);

	//弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//getter
	bool GetIsInit() const { return isInit; }

private:
	//入力
	Input* input = nullptr;
	// モデル
	Model* playerModel = nullptr;

	//弾種類
	int bulletNum = 0;

	//弾
	PlayerBullet* playerBullet;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//打ち出すまでの時間
	float dalayTimer = 0.0f;

	//登場モーションの時間
	float initMotionTime;

	//攻撃
	bool isBurst = false;

	bool isInit = false;

	//移動時の傾き
	bool isRightRotation;
	bool isLeftRotation;
	bool isUpRotation;
	bool isDownRotation;
};
