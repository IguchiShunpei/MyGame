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
	SetScale(Vector3(1000.0f, 1000.0f, 1000.0f));
}
