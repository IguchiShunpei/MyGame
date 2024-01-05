/**
* @file GameOverScene.cpp
* @brief ゲームオーバー画面のシーンクラス
* @author イグチ_シュンペイ
*/

#include "GameOverScene.h"

using namespace DirectX;

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//天球
	sky_ = new SkyDome;
	sky_->SkyDomeInitialize();

	//カメラ初期化
	viewProjection_ = new ViewProjection();
	viewProjection_->Initialize();
	viewProjection_->SetEye(Vector3(0.0f, 0.0f, 20.0f));

	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice());

	//ゲームオーバーロゴ
	gameOverLogo_.Initialize(dxCommon_->GetDevice(), 0, Vector2(0.0f, 0.0f), false, false);
	gameOverLogo_.SetScale(Vector2(800, 300));
	gameOverLogo_.SetPosition({ 250,0,0 });
	gameOverLogo_.SpriteTransferVertexBuffer(gameOverLogo_, 0);
	gameOverLogo_.Update(gameOverLogo_, spriteCommon_);
	gameOverLogo_.LoadTexture(spriteCommon_, 0, L"Resources/2d/gameOver.png", dxCommon_->GetDevice());

	//スペースキー
	space_.Initialize(dxCommon_->GetDevice(), 1, Vector2(0.0f, 0.0f), false, false);
	space_.SetScale(Vector2(400, 120));
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
	
	isUp_ = true;
	isToTitle_ = false;
	toTitleTimer_ = 0;
}

void GameOverScene::Update()
{

	//天球
	sky_->Update();
	viewProjection_->UpdateMatrix();

	//ロゴやUI
	if (isUp_ == true)
	{
		logoY_ = 15.0f * MathFunc::easeOutSine(logoTime_ / 20.0f);
		logoTime_++;
		if (logoTime_ >= 40)
		{
			isUp_ = false;
			logoTime_ = 0;
		}
	}
	else
	{
		logoY_ = 15.0f * -MathFunc::easeOutSine(logoTime_ / 20.0f);
		logoTime_++;
		if (logoTime_ >= 40)
		{
			isUp_ = true;
			logoTime_ = 0;
		}
	}
	gameOverLogo_.SetPosition({ 250,30 + logoY_,0 });
	//ロゴやUI
	gameOverLogo_.Update(gameOverLogo_, spriteCommon_);
	space_.Update(space_, spriteCommon_);
	black_.Update(black_, spriteCommon_);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) == true)
	{
		isToTitle_ = true;
	}
	if (isToTitle_ == true)
	{
		if (blackAlpha_ < blackAlphaNumMax_)
		{
			blackAlpha_ += blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
		toTitleTimer_++;
		if (toTitleTimer_ >= 100)
		{
			//TITLE（次シーン）を生成
			GameSceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
	else
	{
		if (blackAlpha_ > blackAlphaNumMin_)
		{
			blackAlpha_ -= blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
	}
}

void GameOverScene::Draw()
{
	// 描画前処理
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	//天球
	sky_->Draw(viewProjection_);

	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon_->GetCommandList(), spriteCommon_);

	gameOverLogo_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	space_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	black_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	// 描画後処理
	dxCommon_->PostDraw();
}

void GameOverScene::Finalize()
{
	
}
