#include "GameClearScene.h"

using namespace DirectX;

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
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

	//黒
	black_ = new Black;
	black_->BlackInitialize();
	black_->SetPosition(Vector3(0.0f, 0.0f, 21.0f));

	space_ = new Sprite;
	space_->Initialize(dxCommon_);
	space_->LoadTexture(0, L"Resources/2d/space.png", dxCommon_);
	space_->SetScale({ 4,1 });
	space_->SetPosition({ 450,550,0 });

	gameClearLogo_ = new Sprite;
	gameClearLogo_->Initialize(dxCommon_);
	gameClearLogo_->LoadTexture(0, L"Resources/2d/gameClear01.png", dxCommon_);
	gameClearLogo_->SetScale({ 10,4 });
	gameClearLogo_->SetPosition({ 170,-60,0 });

	isUp_ = true;
	isToTitle_ = false;
	toTitleTimer_ = 0;
}

void GameClearScene::Update()
{
	black_->BlackUpdate();
	//天球
	sky_->Update();
	viewProjection_->UpdateMatrix();

	space_->Update();

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
	gameClearLogo_->SetPosition({ 160,logoY_,0 });
	gameClearLogo_->Update();
	black_->BlackUpdate();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) == true)
	{
		isToTitle_ = true;
	}
	if (isToTitle_ == true)
	{
		black_->SetIsFinish(true);
		toTitleTimer_++;
		if (toTitleTimer_ >= 100)
		{
			//TITLE（次シーン）を生成
			GameSceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
}

void GameClearScene::Draw()
{
	// 描画前処理
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky_->Draw(viewProjection_);

	Object3d::PostDraw();

	space_->SetTextureCommands(0, dxCommon_);
	space_->Draw(dxCommon_);
	gameClearLogo_->SetTextureCommands(0, dxCommon_);
	gameClearLogo_->Draw(dxCommon_);

	Object3d::PreDraw(dxCommon_->GetCommandList());

	black_->BlackDraw(viewProjection_);

	Object3d::PostDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void GameClearScene::Finalize()
{

}
