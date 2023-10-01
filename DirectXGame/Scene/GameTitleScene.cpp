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
	player->SetPosition(Vector3(0.0f, -2.0f, 0.0f));

	//カメラ初期化
	viewProjection = new ViewProjection();
	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
	viewProjection->SetTarget(player->worldTransform_.GetPosition());

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//タイトルカメラワーク
	titleNum = 0;
	isTitleCameraWork_ = false;
	titleTimer_ = 0;
	plRotaY = 0.0f;
	skyRotaY = 0.0f;

	//フラグ
	isBeforeCameraWork_ = false;
	isTitleCameraWork_ = false;
}

void GameTitleScene::Update()
{
	//待機時
	if (isBeforeCameraWork_ == false)
	{
		viewProjection->SetTarget(player->GetWorldPosition());
		if (player->worldTransform_.rotation_.y <= 360.0f)
		{
			player->worldTransform_.rotation_.y += 0.4f;
			plRotaY = player->worldTransform_.rotation_.y;
		}
		else
		{
			player->worldTransform_.rotation_.y = 0.0f;
		}
		if (sky->worldTransform_.rotation_.y <= 360.0f)
		{
			sky->worldTransform_.rotation_.y += 0.05f;
			skyRotaY = sky->worldTransform_.rotation_.y;
		}
		else
		{
			sky->worldTransform_.rotation_.y = 0.0f;
		}
		if (input_->TriggerKey(DIK_SPACE))
		{
			isBeforeCameraWork_ = true;
		}
		player->worldTransform_.UpdateMatrix();
	}
	else
	{
		if (isTitleCameraWork_ == false)
		{
			if (titleTimer_ <= 90)
			{
				titleTimer_++;
				player->worldTransform_.rotation_.y = 360 * MathFunc::easeOutSine(titleTimer_ / 90.0f);
				sky->worldTransform_.rotation_.y = 360 * MathFunc::easeOutSine(titleTimer_ / 90.0f);
			}
			else
			{
				titleTimer_ = 0;
				isTitleCameraWork_ = true;
				player->worldTransform_.rotation_.y = 0.0f;
				sky->worldTransform_.rotation_.y = 0.0f;
			}
			player->worldTransform_.UpdateMatrix();
		}
	}

	//カメラワークへ
	if (isTitleCameraWork_ == true)
	{
		titleTimer_++;
		//タイマーが60になったらtitleNumを+する
		if (titleTimer_ == 60)
		{
			titleNum++;
			titleTimer_ = 0;
			//カメラの位置を切り替える
			switch (titleNum)
			{
			case 1:
				viewProjection->SetTarget(player->GetWorldPosition());
				viewProjection->SetEye(Vector3(0.0f, 0.0f, 7.0f));
				break;
			case 2:
				viewProjection->SetTarget(player->GetWorldPosition());
				viewProjection->SetEye(Vector3(0.0f, 2.0f, -14.0f));
				break;
			case 3:
				viewProjection->SetTarget(Vector3(0.0f, 0.0f, 100.0f));
				viewProjection->SetEye(Vector3(0.0f, -1.0f, 0.0f));
				break;
			}
		
		}
		StartCameraWork(titleNum);
		if (titleNum == 4)
		{
			GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
	//天球
	sky->Update();
	viewProjection->UpdateMatrix();
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
		viewProjection->SetEye(Vector3(0.0f, 0.0f, 10.0f));
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