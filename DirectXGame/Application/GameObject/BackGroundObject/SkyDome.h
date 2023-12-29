/**
* @file SkyDome.cpp
* @brief 背景の天球の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once 
#include "Object3d.h"

class SkyDome : public Object3d
{
public:
	//デストラクタ
	~SkyDome();
	//初期化
	void SkyDomeInitialize();

	void SkyDomeUpdate();

private:
	// モデル
	Model* skyModel = nullptr;
};