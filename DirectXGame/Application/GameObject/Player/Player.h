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

	//振動
	void Shake(float x,float y);

	//移動
	void Move();

	//速度変化
	void SpeedChange();

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

	//弾リスト
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//移動速度
	Vector3 speed_;
	float speedU_;
	float speedD_;
	float speedR_;
	float speedL_;
	//加減速値
	float speedChange_;
	//最高速度
	float speedMax_;

	//弾種類
	int bulletNum_;
	//体力
	int hp_;
	//弾の速度
	float kBulletSpeed_;
	//弾の向き
	int bulletDir_;

	//登場モーションに使う変数
	float initSpeedZ_;
	float initMotionTimeMax_;
	float initRotaZ_;

	//タイマー
	//打ち出すまでの時間
	float dalayTimer_;
	//登場モーションの時間
	float initMotionTime_;

	//フラグ
	//移動
	bool isMoveUp_;
	bool isMoveDown_;
	bool isMoveRight_;
	bool isMoveLeft_;
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
	//カメラの向きによって移動方向を変えるフラグ
	bool isChangeDir_;
};
