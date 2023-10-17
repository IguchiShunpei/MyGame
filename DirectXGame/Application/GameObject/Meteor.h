#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"

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

	//浮上
	void Levitation();

	//setter
	void SetBeforeY(float beforeY) { this->beforeY_ = beforeY; }

private:
	//座標
	Vector3 position_;
	//速度
	float speed_;
	//手前
	float frontNum_;
	//奥
	float backNum_;
	//高さ
	float meteorY_;
	//前のy座標
	float beforeY_;
	//浮上モーションの時間
	float levTime_;
	//範囲
	float levRange_;
	//上昇しているか
	bool isUp_;
	//回転の向き
	int rotaNum_;
	//回転の速度
	float rotaSpeed_;
};