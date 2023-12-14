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

	//HP
	void SetHp(int hp) { this->hp_ = hp; }

private:
	//座標
	Vector3 position_;

	//速度
	float speed_;

	//回転速度
	float rotaSpeed_;

	int isRota_;

	//手前
	float frontZ_;

	//消去フラグ
	bool isDelete_;

	//線引きするサイズ
	Vector3 size_;

	//当たったか
	bool isHit_;
	bool isHitEnd_;

	bool isInit_;

	//死亡
	bool isDead_;

	//Hp
	int hp_;

	Vector3 color_;

	float alpha_;

	// モデル
	Model* meteorModel_ = nullptr;
};