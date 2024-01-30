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

	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice());

	//タイトルロゴ
	titleLogo_.Initialize(dxCommon_->GetDevice(), 0, Vector2(0.0f, 0.0f), false, false);
	titleLogo_.SetScale(Vector2(800, 300));
	titleLogo_.SetPosition({ 250,-30,0 });
	titleLogo_.SpriteTransferVertexBuffer(titleLogo_, 0);
	titleLogo_.Update(titleLogo_, spriteCommon_);
	titleLogo_.LoadTexture(spriteCommon_, 0, L"Resources/2d/title01.png", dxCommon_->GetDevice());

	//スペースキー
	space_.Initialize(dxCommon_->GetDevice(), 1, Vector2(0.0f, 0.0f), false, false);
	space_.SetScale(Vector2(400 , 100));
	space_.SetPosition({ 450,500,0 });
	space_.SpriteTransferVertexBuffer(space_, 1);
	space_.Update(space_, spriteCommon_);
	space_.LoadTexture(spriteCommon_, 1, L"Resources/2d/Space.png", dxCommon_->GetDevice());

	//黒
	blackAlpha_ = 1.0f;
	blackAlphaNum_ = 0.02f;
	blackAlphaNumMax_ = 1.0f;
	blackAlphaNumMin_ = 0.0f;
	black_.Initialize(dxCommon_->GetDevice(), 2, Vector2(0.0f, 0.0f), false, false);
	black_.SetScale(Vector2(1280 * 1, 720 * 1));
	black_.SetPosition({ 0,0,0 });
	black_.SpriteTransferVertexBuffer(black_, 2);
	black_.SetAlpha(black_, blackAlpha_);
	black_.Update(black_, spriteCommon_);
	black_.LoadTexture(spriteCommon_, 2, L"Resources/2d/black.png", dxCommon_->GetDevice());

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
		//黒フェードアウト
		if (blackAlpha_ > blackAlphaNumMin_)
		{
			blackAlpha_ -= blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
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
	titleLogo_.Update(titleLogo_, spriteCommon_);
	space_.Update(space_, spriteCommon_);
	black_.Update(black_, spriteCommon_);
}

void GameTitleScene::Draw()
{
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection_);
	player->Draw(viewProjection_);

	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon_->GetCommandList(), spriteCommon_);

	titleLogo_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	space_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	black_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	dxCommon_->PostDraw();
}

void GameTitleScene::Finalize()
{
	delete sky;
	delete player;
	delete viewProjection_;
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
		//黒フェードイン
		if (blackAlpha_ < blackAlphaNumMax_)
		{
			blackAlpha_ += blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
		black_.Update(black_, spriteCommon_);
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
		titleLogo_.SetPosition({ 250,-30 - logoY_,0 });
		space_.SetPosition({ 450,500 + logoY_,0 });
	}
}
