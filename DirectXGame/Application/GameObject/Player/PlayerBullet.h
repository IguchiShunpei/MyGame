/**
* @file PlayerBullet.cpp
* @brief 自機の弾の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Object3d.h"
#include "Model.h"
#include "MathFunc.h"
#include <cmath>
#include <iostream>

class PlayerBullet : public Object3d
{
public:
	//デストラクタ
	~PlayerBullet();
	//初期化
	void PlayerBulletInitialize(const Vector3& position,int bulletLevel,float lengthZ);
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
	Vector3 GetBulletColor() const { return bulletColor_; }
	//ラジアン角
	float GetAngle(float mx, float my, float sx, float sy);

	//bulletNumのsetter
	void SetBulletNum(int bulletNum) { this->bulletNum_ = bulletNum; }

private:

	// モデル
	std::unique_ptr<Model> playerBulletModel_;
	//削除フラグ
	bool isDelete_ = false;
	//弾種類
	int bulletNum_;
	//削除タイマー
	float deleteTimer_;
	//速度
	Vector3 velocity_;
	//当たったか
	bool isHit_ = false;
	//倒したか
	bool isBurst_ = false;

	Vector3 bulletColor_;

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