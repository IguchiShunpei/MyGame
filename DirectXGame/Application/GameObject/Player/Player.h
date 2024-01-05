/**
* @file Player.cpp
* @brief 自機の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "input.h"
#include "Model.h"
#include "Sprite.h"
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
	void Update(ViewProjection* viewProjection_);

	//当たり判定更新
	void ColliderUpdate();

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//登場モーション
	void InitMotion();

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

	//キャラの向きに応じた方向に球を出す
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);
	//ベクトルと行列を掛け算
	Vector3 MatVector(Vector3 v, Matrix4 mat);
	//ビューポート行列をセット
	Matrix4 SetViewport(const Vector3& v);
	//ワールド→スクリーン座標変換
	Vector3 Division(const Vector3& v, Matrix4 mat);
	//ダメージ
	void Damage();

	//弾の威力の変更
	void BulletPowerUp();

	//弾更新
	void BulletUpdate();

	//レティクル更新
	void ReticleUpdate(ViewProjection* viewProjection_);

	//レティクル描画
	void ReticleDraw();

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
	//bulletLevel
	int GetBulletLevel() const { return bulletLevel_; }
	//bulletDamage
	int GetBulletPower()const { return bulletPower_; };

	//setter
	//IsHit
	void SetIsHit(bool isHit) { isHit_ = isHit; }
	//IsInit
	void SetIsInit(bool isInit) { isInit_ = isInit; }
	//IsInv
	void SetIsInv(bool isInv) { isInv_ = isInv; }
	//IsChangeDir
	void SetIsChangeDir(bool isChangeDir) { isChangeDir_ = isChangeDir; }
	//bulletLevel
	void SetBulletLevel(int bulletLevel) { bulletLevel_ = bulletLevel; }
	//bulletPower
	void SetBulletPower(int bulletPower) { bulletPower_ = bulletPower; }

private:
	//入力
	Input* input_ = nullptr;
	//DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// モデル
	Model* playerModel_ = nullptr;

	//弾
	PlayerBullet* playerBullet_ = nullptr;

	//弾リスト
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//レティクル
	WorldTransform worldTransform3DReticle_;
	//スプライト
	Sprite* sprite_;
	SpriteCommon spriteCommon_;
	//2Dレティクル用スプライト
	Sprite reticle_;
	//自機からレティクルまでの距離
	float distancePlayerToReticle_;
	//レティクルalpha値
	float reticleAlpha_;
	//変動値
	float reticleAlphaNum_;
	//最大値
	float reticleAlphaNumMax_;
	//最小値
	float reticleAlphaNumMin_;
	//レティクル表示フラグ
	bool isInvicibleReticle_;

	//collider関係
	Vector3 colliderPos_;
	float playerColliderRadius_;
	float bulletColliderRadius_;

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
	//回転値
	float moveRota_;
	//最大角度
	float moveRotaMax_;

	//弾種類
	int bulletLevel_;
	int bulletDamage_;
	//体力
	int hp_;
	//弾の速度
	float kBulletSpeed_;
	//弾の向き
	int bulletDir_;
	//弾の威力
	int bulletPower_;

	//登場モーションに使う変数
	float initSpeedZ_;
	float initMotionTimeMax_;
	float initRotaZ_;
	//登場座標
	Vector3 initPos_;
	//角度
	Vector3 initRota_;
	//回転を戻す
	float backRota_;
	//登場時の最大角度
	float initRotaMax_;

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
