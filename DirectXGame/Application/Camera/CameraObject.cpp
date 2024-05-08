/**
* @file  CameraObject.cpp
* @brief 始点と終点のオブジェクト
* @author イグチ_シュンペイ
*/

#include "CameraObject.h"

void CameraObject::CObjectInitialize()
{
	// OBJからモデルデータを読み込む
	for (int i = 0; i < 2; i++)
	{
	cameraObjectModel_[i] = Model::LoadFromOBJ("cameraObject");
		//生成
		cameraObject_[i] = std::make_unique <Object3d>();
		//初期化
		cameraObject_[i]->Initialize();
		// 3Dオブジェクト生成
		cameraObject_[i]->Create();
		// オブジェクトにモデルをひも付ける
		cameraObject_[i]->SetModel(cameraObjectModel_[i].get());
	}
	cameraObject_[0]->SetPosition({ 0.0f, 0.0f, 0.0f });
	cameraObject_[1]->SetPosition({ 10.0f, 0.0f, 0.0f });
}

void CameraObject::CObjectUpdate()
{
	for (int i = 0; i < 2; i++)
	{
		//更新
		cameraObject_[i]->Update();
	}
}

void CameraObject::CObjectDraw(ViewProjection* viewProjection)
{
	//始点と終点を描画
	cameraObject_[0]->Draw(viewProjection, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
	cameraObject_[1]->Draw(viewProjection, 1.0f, Vector3(1.0f, 0.0f, 0.0f));
}

void CameraObject::Reset()
{
	cameraObject_[0]->SetPosition({ 0.0f, 0.0f, 0.0f });
	cameraObject_[1]->SetPosition({ 10.0f, 0.0f, 0.0f });
}
