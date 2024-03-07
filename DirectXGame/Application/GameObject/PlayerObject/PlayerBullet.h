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
	void BulletInitialize(const Vector3& position,Vector3 velocity,int bulletLevel);
	void LaserInitialize();
	//当たり判定更新
	void ColliderUpdate();

	//更新
	void BulletUpdate();
	void LaserUpdate(Vector3 position, Vector3 rotation);

	//レーザー登場
	void LaserInit(Vector3 position, Vector3 rotation);
	//消去
	void LaserOut();
	//点滅
	void Blining();

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//getter
	bool GetIsDelete() const { return isDelete_; }
	bool GetISHit() const { return isHit_; }
	Vector3 GetBulletColor() const { return bulletColor_; }
	float GetAlpha()const { return alpha_; };
	
	//bulletNumのsetter
	void SetBulletNum(int bulletNum) { this->bulletNum_ = bulletNum; }
	void SetIsLaserInit(bool isInit) { this->isInit_ = isInit; }
	void SetIsLaserBack(bool isBack) { this->isBack_ = isBack; }
	void SetIsBlink(bool isBlink) { this->isBlink_ = isBlink; }

private:

	// モデル
	std::unique_ptr<Model> laserModel_;
	std::unique_ptr<Model> playerBulletModel_;
	//削除フラグ
	bool isDelete_ = false;
	//弾種類
	int bulletNum_;
	//速度
	Vector3 velocity_;
	//当たったか
	bool isHit_ = false;
	//消える位置
	float deleteZ_;
	//弾の色
	Vector3 bulletColor_;
	//拡大数値
	Vector3 scaleNum_;
	//回転量
	float rotaNum_;

	//レーザー
	//スケール
	float laserScale_;
	float laserScaleMin_;
	float laserScaleMax_;
	//登場フラグ
	bool isInit_;
	//退場フラグ
	bool isBack_;
	//登場
	float initTime_;
	float initTimeMax_;
	//回転
	float laserRota_;
	float laseRotaMax_;
	//点滅フラグ
	bool isBlink_;
	bool isOn_;
	//点滅時間
	float blinkTime_;
	float blinkTimeMax_;

	//alpha
	float alpha_;
	float alphaMax_;
	float alphaMin_;
	float alphaNum_;
};