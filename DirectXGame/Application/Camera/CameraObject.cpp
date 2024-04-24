/**
* @file  CameraObject.cpp
* @brief 始点と終点のオブジェクト
* @author イグチ_シュンペイ
*/

#include "CameraObject.h"

void CameraObject::CObjectInitialize()
{
	Initialize();

	// OBJからモデルデータを読み込む
	cameraObjectModel_ = Model::LoadFromOBJ("cameraObject");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(cameraObjectModel_.get());
}
