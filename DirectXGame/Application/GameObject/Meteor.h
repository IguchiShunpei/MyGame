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

private:
	Vector3 position;
};