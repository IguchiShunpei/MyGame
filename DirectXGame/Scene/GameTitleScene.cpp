#include "GameTitleScene.h"

using namespace DirectX;

GameTitleScene::GameTitleScene()
{

}

GameTitleScene::~GameTitleScene()
{
}

void GameTitleScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//カメラ初期化
	viewProjection = new ViewProjection();
	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
	viewProjection->SetTarget(player->GetWorldPosition());

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//タイトルカメラワーク
	titleNum = 0;
	isTitleCameraWork_ = false;
	titleTimer_ = 0;
}

void GameTitleScene::Update()
{
	//天球
	sky->Update();
	viewProjection->UpdateMatrix();

	if (isTitleCameraWork_ == false)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			isTitleCameraWork_ = true;
			player->worldTransform_.rotation_.y = 0.0f;
			sky->worldTransform_.rotation_.y = 0.0f;
		}
	}
	else
	{
		//タイマーが60になったらtitleNumを+する
		titleTimer_++;
		if (titleTimer_ == 60)
		{
			titleNum++;
			titleTimer_ = 0;
			//カメラの位置を切り替える
			switch (titleNum)
			{
			case 1:
				viewProjection->SetEye(Vector3(0.0f, 0.0f, 10.0f));
				break;
			case 2:
				viewProjection->SetEye(Vector3(0.0f, 2.0f, -20.0f));
				break;
			case 3:
				viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
				viewProjection->SetTarget(Vector3(0.0f, 0.0f, 100.0f));
				break;
			}
		}
		StartCameraWork(titleNum);
		if (titleNum == 4)
		{
			player->SetPosition(Vector3(0, -2, -20));
			viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
			titleNum = 0;
			titleTimer_ = 0;
			isTitleCameraWork_ = false;
			GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
}

void GameTitleScene::Draw()
{
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection);
	player->Draw(viewProjection);

	Object3d::PostDraw();

	dxCommon_->PostDraw();

}

void GameTitleScene::Finalize()
{

}

void GameTitleScene::StartCameraWork(int num)
{
	switch (num)
	{
	case 0:
		viewProjection->eye.x = 15.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		viewProjection->eye.y = 10.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 1:
		viewProjection->eye.x = 20.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 2:
		viewProjection->eye.z = 30.0f * MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 3:
		//自機を動かす
		player->worldTransform_.position_.z++;
		viewProjection->eye.z -= 1.5f;
		player->worldTransform_.rotation_.z = 360.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		break;
	}
}