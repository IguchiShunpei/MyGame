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

	//スケール
	void Scale();

	//サイズを設定
	void SetSize();

	//移動
	void Move();

	//回転
	void Rotate();

	//浮上
	void Levitate();

	//setter
	void SetBeforeY(float beforeY) { this->beforeY_ = beforeY; }

private:
	//座標
	Vector3 position_;
	//速度
	float speed_;
	//手前
	float frontX_;
	//奥
	float backZ_;
	//高さ
	float meteorY_;
	//前のy座標
	float beforeY_;
	//浮上モーションの時間
	float levTime_;
	//浮上する向き
	int levDirection_;
	//範囲
	float levRange_;
	//上昇しているか
	bool isUp_;
	//回転の向き
	int rotaDirection_;
	//回転の速度
	float rotaSpeed_;
	//サイズ乱数を受け取る変数
	int meteorSize_;
	//サイズ
	Vector3 size_;
};