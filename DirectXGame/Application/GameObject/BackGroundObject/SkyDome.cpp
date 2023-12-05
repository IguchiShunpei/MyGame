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

void SkyDome::SkyDomeUpdate()
{
	worldTransform_.position_.z -= 0.1f;
	Update();
}
