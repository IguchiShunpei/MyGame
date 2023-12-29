/**
* @file GameTitleScene.cpp
* @brief タイトル画面のシーンクラス
* @author イグチ_シュンペイ
*/

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
	viewProjection_ = new ViewProjection();
	viewProjection_->Initialize();
	viewProjection_->SetEye(Vector3(0.0f, 4.0f, 18.0f));
	viewProjection_->SetTarget(Vector3(0.0f, -2.0f, 0.0f));

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	black_ = new Black;
	black_->BlackInitialize();
	black_->SetPosition(Vector3(0.0f, 4.0f, 17.0f));

	titleLogo_ = new Sprite;
	titleLogo_->Initialize(dxCommon_);
	titleLogo_->LoadTexture(0, L"Resources/2d/title01.png", dxCommon_);
	titleLogo_->SetScale({ 10,4 });
	titleLogo_->SetPosition({ 170,-60,0 });

	space_ = new Sprite;
	space_->Initialize(dxCommon_);
	space_->LoadTexture(0, L"Resources/2d/space.png", dxCommon_);
	space_->SetScale({ 4,1 });
	space_->SetPosition({ 450,550,0 });

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
	black_->BlackUpdate();
	//待機時
	if (isBeforeCameraWork_ == false)
	{
		viewProjection_->SetTarget(player->GetWorldPosition());
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
		LogoOut();
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
				viewProjection_->SetTarget(player->GetWorldPosition());
				viewProjection_->SetEye(Vector3(0.0f, 0.0f, 7.0f));
				break;
			case 2:
				viewProjection_->SetTarget(player->GetWorldPosition());
				viewProjection_->SetEye(Vector3(0.0f, 2.0f, 1.0f));
				cameraWorkPos_ = viewProjection_->eye_;
				break;
			case 3:
				viewProjection_->SetTarget(Vector3(0.0f, 0.0f, 100.0f));
				viewProjection_->SetEye(Vector3(0.0f, -1.0f, 0.0f));
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
	viewProjection_->UpdateMatrix();

	//ロゴやUI
	titleLogo_->Update();
	space_->Update();
}

void GameTitleScene::Draw()
{
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection_);
	player->Draw(viewProjection_);

	Object3d::PostDraw();

	titleLogo_->SetTextureCommands(0, dxCommon_);
	titleLogo_->Draw(dxCommon_);

	space_->SetTextureCommands(0, dxCommon_);
	space_->Draw(dxCommon_);

	Object3d::PreDraw(dxCommon_->GetCommandList());

	black_->BlackDraw(viewProjection_);

	Object3d::PostDraw();

	dxCommon_->PostDraw();
}

void GameTitleScene::Finalize()
{
	delete sky;
	delete player;
	delete viewProjection_;
	delete titleLogo_;
	delete space_;
	delete black_;
}

void GameTitleScene::StartCameraWork(int num)
{
	switch (num)
	{
	case 0:
		viewProjection_->SetEye(Vector3(0.0f, 0.0f, 10.0f));
		viewProjection_->eye_.x = 15.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		viewProjection_->eye_.y = 10.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 1:
		viewProjection_->eye_.x = 20.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 2:
		viewProjection_->eye_.z = cameraWorkPos_.z + 30.0f * MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 3:
		black_->SetIsFinish(true);
		//自機を動かす
		player->worldTransform_.position_.z++;
		viewProjection_->eye_.z -= 1.5f;
		player->worldTransform_.rotation_.z = 360.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		break;
	}
}

void GameTitleScene::LogoOut()
{
	if (logoTime_ <= 30.0f)
	{
		logoY_ = 300.0f * MathFunc::easeInSine(logoTime_ / 30.0f);
		logoTime_++;
		titleLogo_->SetPosition({ 170,-60 - logoY_,0 });
		space_->SetPosition({ 450,550 + logoY_,0 });
	}
}
