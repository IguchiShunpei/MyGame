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
	SetScale(Vector3(454.0f, 340.0f, 500.0f));
}
