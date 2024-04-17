#include "GameScene.h"

void GameScene::Initialize()
{
	input_ = SIEngine::Input::GetInstance();
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	//カメラ初期化
	//viewProjection_ = std::make_unique <ViewProjection>();
	//viewProjection_->Initialize();
	//viewProjection_->SetEye(Vector3(0.0f, 4.0f, 18.0f));
	//viewProjection_->SetTarget(Vector3(0.0f, -2.0f, 0.0f));

	//天球
	sky_ = std::make_unique < SkyDome>();
	sky_->SkyDomeInitialize();
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}

void GameScene::Finalize()
{
}
