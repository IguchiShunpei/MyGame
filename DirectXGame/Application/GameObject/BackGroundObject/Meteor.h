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

	bool GetIsDelete() const { return isDelete_; };

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
};