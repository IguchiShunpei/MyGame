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

	//待機モーション
	void Levitate();

	//移動
	void Move();

	//傾き
	void Rotate();

	//攻撃
	void Attack();

	//ダメージ
	void Damage();

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
	//IsDead
	bool GetIsDead() const { return isDead_; }
	//IsInv
	bool GetIsInv() const { return isInv_; }

	//setter
	//IsHit
	void SetIsHit(bool isHit) { isHit_ = isHit; }
	//IsInit
	void SetIsInit(bool isInit) { isInit_ = isInit; }
	//IsInv
	void SetIsInv(bool isInv) { isInv_ = isInv; }
	//IsChangeDir
	void SetIsChangeDir(bool isChangeDir) { isChangeDir_ = isChangeDir; }

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
	//体力
	int hp_;
	//浮上する向き
	int levDirection_;
	//範囲
	float levRange_;
	//待機モーションのy変数
	float playerY_;
	float beforeY_;
	//弾の速度
	float kBulletSpeed_;
	//弾の向き
	int bulletDir_;

	//タイマー
	//打ち出すまでの時間
	float dalayTimer_;
	//登場モーションの時間
	float initMotionTime_;
	//浮上モーションの時間
	float levTime_;

	//フラグ
	//倒したか
	bool isBurst_;
	//登場したか
	bool isInit_;
	bool isInitAfter_;
	//敵の攻撃に当たったか
	bool isHit_;
	bool isInv_;
	//死亡
	bool isDead_;
	//上昇しているか
	bool isUp_;
	//移動しているか
	bool isMove_;
	//カメラの向きによって移動方向を変えるフラグ
	bool isChangeDir_;

	//移動時の傾き
	bool isRightRotation_;
	bool isLeftRotation_;
	bool isUpRotation_;
	bool isDownRotation_;
};
