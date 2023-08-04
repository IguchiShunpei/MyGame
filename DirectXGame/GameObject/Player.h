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

//GameSceneの前方宣言
class GamePlayScene;

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

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

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

	//弾更新
	void BulletUpdate();

	//弾描画
	void BulletDraw(ViewProjection* viewProjection_);

	//弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//getter
	//IsInit
	bool GetIsInit() const { return isInit_; }
	//IsHit
	bool GetIsHit() const { return isHit_; }

	//setter
	//IsHit
	void SetIsHit(bool isHit) { isHit_ = isHit; }

private:
	//入力
	Input* input_ = nullptr;
	// モデル
	Model* playerModel_ = nullptr;

	//弾
	PlayerBullet* playerBullet_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//弾種類
	int bulletNum_;

	//タイマー
	//打ち出すまでの時間
	float dalayTimer_;
	//登場モーションの時間
	float initMotionTime_;

	//フラグ
	//倒したか
	bool isBurst_;
	//登場したか
	bool isInit_;
	//敵の攻撃に当たったか
	bool isHit_;

	//移動時の傾き
	bool isRightRotation_;
	bool isLeftRotation_;
	bool isUpRotation_;
	bool isDownRotation_;
};
