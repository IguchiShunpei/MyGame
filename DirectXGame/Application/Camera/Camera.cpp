/**
* @file Camera.cpp
* @brief カメラ挙動のクラス
* @author イグチ_シュンペイ
*/

#include "Camera.h"
#include <math.h>
#include <random>

UI* Camera::ui_ = nullptr;
Player* Camera::player_ = nullptr;

void Camera::Initialize()
{
	input_ = SIEngine::Input::GetInstance();
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	changeTarget_ = 0.3f;

	//カメラ初期化
	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->SetEye({ -15.0f,0.0f,10.0f });
	viewProjection_->SetUp({ 0.0f,1.0f,0.0f });
	startCameraPos_ = viewProjection_->GetEye();
	startCamoreMovePos_ = { 15.0f,5.0f,30.0f };
	beforeTargetNum_ = viewProjection_->target_;
	normalTargetNum_ = viewProjection_->target_;
	normalUpNum_ = viewProjection_->up_;

	//カメラワーク番号
	bossInitNum_ = BossInitNum::Up;

	cameraShakePos_ = { 0.0f,0.0f,0.0f };
	deadCameraPos_ = { 0.0f,0.0f,0.0f };
	deadCameraMovePos_ = { 20.0f ,0.0f,35.0f };
	bossInitCameraPos_ = { 0.0f,0.0f,0.0f };

	//基本target
	changeTargetNum_ = { 0.0f,0.0f,0.0f };
	//登場した後にtargetを戻す値
	changeTargetMoveNum_ = { 0.0f,0.0f,50.0f };

	//登場した後の各座標
	afterInitCameraPos_ = { 0.0f, 5.0f, -20.0f };
	afterInitCameraTarget_ = { 0.0f, -2.0f, 50.0f };

	allZero_ = { 0.0f,0.0f,0.0f };

	//シェイク
	bossCameraShake_ = 1.5f;

	//ボス登場
	upZ_ = 10.0f;
	looseZ_ = 1.0f;

	clearCameraNum_ = 0;

	//タイマー
	bossAppTimer_ = 0.0f;
	bossAppTimerMax_ = 180.0f;
	clearTimer_ = 0.0f;
	bossInitTimer_ = 0;
	bossInitTimerMax_ = 24;
	startTimer_ = 0.0f;
	startTimerMax_ = 120.0f;
	cameraMoveTimer_ = 0.0f;
	cameraMoveTimerMax_ = 60.0f;
	playerDeadTimer_ = 0.0f;
	playerDeadTimerMax_ = 60.0f;
	targetMoveTimer_ = 0.0f;
	targetMoveTimerMax_ = cameraMoveTimerMax_ * 2.0f;
	clearCameraTimer_ = 0.0f;
	clearCameraTimerMax_ = 180.0f;
}

void Camera::Update()
{
	viewProjection_->UpdateMatrix();
}

void Camera::PlayerInitCameraWork(bool& isStart,Player*player)
{
	if (isStart == false)
	{
		viewProjection_->SetTarget(player->GetPosition());
		//自機が到着したらカメラを初期位置へ
		if (player->GetIsInit() == true)
		{
			viewProjection_->eye_.x = startCameraPos_.x + startCamoreMovePos_.x * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.y = startCameraPos_.y + startCamoreMovePos_.y * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.z = startCameraPos_.z + startCamoreMovePos_.z * -MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->target_.z = changeTargetNum_.z + changeTargetMoveNum_.z * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			startTimer_++;
			if (startTimer_ > startTimerMax_)
			{
				viewProjection_->SetEye(afterInitCameraPos_);
				viewProjection_->SetTarget(afterInitCameraTarget_);
				beforeEyeNum_ = allZero_;
				beforeTargetNum_ = allZero_;
				beforeUpNum_ = allZero_;
				normalEyeNum_ = viewProjection_->eye_;
				normalTargetNum_ = viewProjection_->target_;
				normalUpNum_ = viewProjection_->up_;
				isStart = true;
			}
		}
	}
}

void Camera::BossInitCameraWork(BossEnemy*bEnemy,bool& isBossInitCamera)
{
	switch (bossInitNum_)
	{
	case None:

		break;
	case Up:
		//カメラを寄せる
		if (viewProjection_->eye_.z <= upZ_)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() + Vector3(0, 0, upZ_) * MathFunc::easeInSine(bossAppTimer_ / bossAppTimerMax_));
		}
		else
		{
			//寄ったら黒フェード
			ui_->SetIsBlack(false);
			ui_->FadeIn();
			if (ui_->GetIsBlack() == true)
			{
				//黒フェードが完了したら登場カメラワークへ
				bossInitNum_ = InitCameraWork;
				bossAppTimer_ = 0;
				viewProjection_->up_ = { 0.0f,1.0f,0.0f };
			}
		}
		bossAppTimer_++;

		break;
	case InitCameraWork:
		//黒フェードアウト
		ui_->FadeOut(ui_->Black);
		viewProjection_->SetTarget(bEnemy->GetPosition());
		bEnemy->InitMotion();

		if (bEnemy->GetIsInit() == true)
		{
			//登場カメラシェイク時間
			bossInitTimer_++;
			if (bossInitTimer_ < bossInitTimerMax_)
			{
				//bossInitTimerが奇数ならカメラシェイク
				if (bossInitTimer_ % 2 != 1)
				{
					CameraShake(bossCameraShake_, bossCameraShake_);
				}
				else
				{
					viewProjection_->SetEye(bossInitCameraPos_);
				}
			}
			else
			{
				bossInitTimer_ = 0;
				viewProjection_->SetEye(bossInitCameraPos_);
				bossInitNum_ = Loose;
			}
		}
		else
		{
			bossInitCameraPos_ = viewProjection_->GetEye();
		}

		break;
	case Loose:
		if (viewProjection_->eye_.z > afterInitCameraPos_.z)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() - Vector3(0, 0, looseZ_));
		}
		else
		{
			viewProjection_->eye_.z = afterInitCameraPos_.z;
			isBossInitCamera = false;
		}
		break;
	}
}

void Camera::ToClearCameraWork(bool& isClearCameraWork)
{
	if (clearCameraTimer_ <= clearCameraTimerMax_)
	{
		switch (clearCameraNum_)
		{
			//自機の周りの点を移動
		case 0:
			viewProjection_->eye_.x = normalEyeNum_.x + (20.0f - normalEyeNum_.x) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			viewProjection_->eye_.z = normalEyeNum_.z + (20.0f - normalEyeNum_.z) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			normalTargetNum_ = viewProjection_->target_;
			break;
		case 1:
			viewProjection_->eye_.x = normalEyeNum_.x + (0.0f - normalEyeNum_.x) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			viewProjection_->eye_.z = normalEyeNum_.z + (-20.0f - normalEyeNum_.z) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			break;
		case 2:
			//1回転したらカメラワークフラグをtrueに
			isClearCameraWork = true;
			viewProjection_->eye_.x = 0.0f;
			viewProjection_->eye_.z = -20.0f;
			break;
		}
		clearCameraTimer_++;
		viewProjection_->UpdateMatrix();
	}
	else
	{
		clearCameraTimer_ = 0.0f;
		clearCameraTimerMax_ = 90.0f;
		normalEyeNum_ = viewProjection_->eye_;
		clearCameraNum_++;
	}
}

void Camera::ToGameOverCameraWork(Player*player,bool& isPlayerDead,bool& isGameOver)
{
	viewProjection_->SetTarget(player->GetPosition());
	if (isPlayerDead == false)
	{
		if (playerDeadTimer_ <= playerDeadTimerMax_)
		{
			viewProjection_->eye_.x = deadCameraPos_.x + deadCameraMovePos_.x * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			viewProjection_->eye_.y = deadCameraPos_.y + deadCameraMovePos_.y * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			viewProjection_->eye_.z = deadCameraPos_.z + deadCameraMovePos_.z * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			playerDeadTimer_++;
		}
		else
		{
			normalEyeNum_ = viewProjection_->eye_;
		}
	}
	else
	{
		isGameOver = true;
	}
}

void Camera::CameraShake(float x, float y)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-x, x);
	std::uniform_real_distribution<float>dist2(-y, y);

	viewProjection_->eye_ = viewProjection_->eye_ + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection_->UpdateMatrix();
}

void Camera::MoveCamera()
{
	//カメラ移動処理
	if (input_->PushKey(DIK_UP))
	{
		ui_->SetIsUp(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, changeTarget_, 0));
	}
	else
	{
		ui_->SetIsUp(false);
		if (viewProjection_->target_.y >= normalTargetNum_.y)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -changeTarget_, 0));
		}
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		ui_->SetIsRight(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(changeTarget_, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(-0.0025f, 0, 0));
	}
	else
	{
		ui_->SetIsRight(false);
		if (viewProjection_->target_.x >= normalTargetNum_.x)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-changeTarget_, 0, 0));
		}
		if (viewProjection_->up_.x <= normalUpNum_.x)
		{
			viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
		}
	}
	if (input_->PushKey(DIK_LEFT))
	{
		ui_->SetIsLeft(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-changeTarget_, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
	}
	else
	{
		ui_->SetIsLeft(false);
		if (viewProjection_->target_.x <= normalTargetNum_.x)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(changeTarget_, 0, 0));
		}
		if (viewProjection_->up_.x >= normalUpNum_.x)
		{
			viewProjection_->SetUp(viewProjection_->up_ + Vector3(-0.0025f, 0, 0));
		}
	}

	if (input_->PushKey(DIK_DOWN))
	{
		ui_->SetIsDown(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -changeTarget_, 0));
	}
	else
	{
		ui_->SetIsDown(false);
		if (viewProjection_->target_.y <= normalTargetNum_.y)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, changeTarget_, 0));
		}
	}

	//移動限界座標
	const float kTargetLimitX = 9.0f;
	const float kTargetLimitY = 7.0f;

	//範囲を超えない処理
	viewProjection_->target_.x = max(viewProjection_->target_.x, -kTargetLimitX);
	viewProjection_->target_.x = min(viewProjection_->target_.x, +kTargetLimitX);
	viewProjection_->target_.y = max(viewProjection_->target_.y, -kTargetLimitY);
	viewProjection_->target_.y = min(viewProjection_->target_.y, +kTargetLimitY);

	const float kUpLimitX = 0.15f;
	viewProjection_->up_.x = max(viewProjection_->up_.x, -kUpLimitX);
	viewProjection_->up_.x = min(viewProjection_->up_.x, +kUpLimitX);
}
