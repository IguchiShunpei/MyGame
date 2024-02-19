/**
* @file Item.cpp
* @brief アイテムの処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "MathFunc.h"
#include <memory>


class Item : public Object3d
{
public:
	//デストラクタ
	~Item();
	//初期化
	void ItemInitialize(Vector3 initPos_);

	//更新
	void ItemUpdate();

	//当たり判定更新
	void ColliderUpdate();

	//getter
	//IsDelete
	bool GetIsDelete() const { return isDelete_; }
	//isHit
	bool GetIsHit() const { return isHit_; }

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//移動
	void Move();

	//回転
	void Rotate();

	//当たったら発動
	void Hit();

	//当たった時の動き
	void HitMotion();

private:
	//デスポーンしたか
	bool isDelete_;
	//当たったか
	bool isHit_;

	//通常時

	//移動速度
	float moveSpeed_;
	//通常時回転速度
	float normalRotaSpeed_;
	//手前
	float frontZ_;

	//ヒット時
	
	//モーション時間
	float HitMotionTime_;
	//最高時間
	float HitMotionTimeMax_;
	//回転速度
	float hitRotaSpeed_;
	//スケール
	Vector3 hitScale_;

	// モデル
	std::unique_ptr <Model> itemModel_;
};