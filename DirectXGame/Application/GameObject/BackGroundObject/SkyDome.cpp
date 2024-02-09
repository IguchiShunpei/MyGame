/**
* @file SkyDome.cpp
* @brief 背景の天球の処理クラス
* @author イグチ_シュンペイ
*/

#include "SkyDome.h"

SkyDome::~SkyDome()
{
	
}

void SkyDome::SkyDomeInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	skyModel = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(skyModel.get());
	SetScale(Vector3(1400.0f, 1400.0f, 1400.0f));
}
