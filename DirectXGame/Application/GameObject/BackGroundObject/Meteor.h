/**
* @file Meteor.h
* @brief 隕石の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include <list>

//GameSceneの前方宣言
class GamePlayScene;

class Meteor : public Object3d
{
public:
	//死亡時効果
	enum DeadEffect
	{
		None,
		Item,
		SmallMeteor
	};
public:
	//初期化
	void MeteorInitialize();

	//更新
	void MeteorUpdate();

	//移動
	void Move();

	//回転
	void Rotate();

	//ダメージ
	void Damage();

	//当たり判定更新
	void ColliderUpdate();

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//削除フラグgetter
	bool GetIsDelete() const { return isDelete_; };
	bool GetIsDead() const { return isDead_; }

	float GetAlpha() const { return alpha_; }

	//Color
	Vector3 GetColor() const { return color_; }

	//死亡効果
	int GetDeadEffect() const { return deadEffect_; }

	//隕石座標
	Vector3 GetSmallMeteorPos(int n) const { return smallMeteorPos_[n]; }
	Vector3 GetVelocity(int n) const { return smallMeteorVel[n]; }

	//HP
	void SetHp(int hp) { this->hp_ = hp; }

	//Alpha
	void SetAlpha(float alpha) { this->alpha_ = alpha; }

	//isBack
	void SetIsBack(bool isBack) { this->isBack_ = isBack; }

	//speed
	void SetSpeed(float min ,float max);

	//deadEffect
	void SetDeadEffect(DeadEffect deadEffect) { this->deadEffect_ = deadEffect; }

	//移動量をセット
	void SetVelocity(Vector3 velocity) { this->velocity_ = velocity; }

private:
	//座標
	Vector3 position_;

	//速度
	Vector3 velocity_;

	//回転速度
	float rotaSpeed_;

	//回転の向き
	int rotaDirection_;

	//手前
	float frontZ_;

	//奥
	float backZ_;

	//消去フラグ
	bool isDelete_;

	//当たったか
	bool isHit_;
	bool isHitEnd_;

	//登場
	bool isInit_;

	//死亡フラグ
	bool isDead_;

	//Hp
	int hp_;

	//色
	Vector3 color_;
	Vector3 hitColor_;

	//alpha
	float alpha_;
	float alphaMax_;

	// モデル
	std::unique_ptr <Model> meteorModel_;

	//背景フラグ
	bool isBack_;

	//死亡時効果
	DeadEffect deadEffect_;

	//小さい隕石の差量
	float smallMeteorlen_ = 1.0f;

	//小さい隕石の座標
	Vector3 smallMeteorPos_[3] = 
	{
		{-smallMeteorlen_,0.0f,0.0f},
		{smallMeteorlen_,0.0f,0.0f},
		{0.0f,smallMeteorlen_,0.0f}
	};

	//速度
	float smallMeteorSpeed_ = 0.1f;;

	Vector3 smallMeteorVel[3] =
	{
		{-smallMeteorSpeed_,-smallMeteorSpeed_,0.0f},
		{smallMeteorSpeed_,-smallMeteorSpeed_,0.0f},
		{0.0f,smallMeteorSpeed_,0.0f},
	};
};