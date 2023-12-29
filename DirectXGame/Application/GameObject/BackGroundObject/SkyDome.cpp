/**
* @file SkyDome.cpp
* @brief 背景の天球の処理クラス
* @author イグチ_シュンペイ
*/

#include "SkyDome.h"

SkyDome::~SkyDome()
{
	delete skyModel;
}

void SkyDome::SkyDomeInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	skyModel = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(skyModel);
	SetScale(Vector3(1500.0f, 1500.0f, 1500.0f));
}
