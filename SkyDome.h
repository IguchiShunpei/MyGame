#pragma once 
#include "Object3d.h"

class SkyDome : public Object3d
{
public:
	//デストラクタ
	~SkyDome();
	//初期化
	void SkyDomeInitialize();

private:
	// モデル
	Model* skyModel = nullptr;
};