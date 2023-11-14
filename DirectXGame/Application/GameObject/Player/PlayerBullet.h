#pragma once
#include "Object3d.h"
#include "Model.h"
#include "MathFunc.h"

class PlayerBullet : public Object3d
{
public:
	//デストラクタ
	~PlayerBullet();
	//初期化
	void PlayerBulletInitialize(const Vector3& position, const Vector3& velocity,int bulletDir);
	//当たり判定更新
	void ColliderUpdate();

	void Update();

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//getter
	bool GetIsDelete() const { return isDelete_; }
	bool GetIsCharge() const { return isCharge_; }
	bool GetISHit() const { return isHit_; }
	bool GetIsBurst() const { return isBurst_; }

	//bulletNumのsetter
	void SetBulletNum(int bulletNum) { this->bulletNum_ = bulletNum; }

private:

	// モデル
	Model* playerBulletModel_ = nullptr;
	//削除フラグ
	bool isDelete_ = false;
	//弾種類
	int bulletNum_ = 0;
	//削除タイマー
	float deleteTimer_ = 110.0f;
	//速度
	Vector3 velocity_;
	//当たったか
	bool isHit_ = false;
	//倒したか
	bool isBurst_ = false;

	//重力
	const float G = 0.3f;
	//初速
	Vector3 startSpeed = { 0.0f,0.0f,0.0f };
	//落下時間
	float flame = 0.0f;

	//溜め時間
	float chargeTime;
	//溜めているか
	bool isCharge_ = false;
	//拡大数値
	Vector3 scaleNum;
};