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
	normalTargetNum_ = viewProjection_->target_;
	normalUpNum_ = viewProjection_->up_;
}

void Camera::Update()
{
	viewProjection_->UpdateMatrix();
}

void Camera::PlayerInitCameraWork()
{
}

void Camera::BossInitCameraWork()
{
}

void Camera::ToClearCameraWork()
{
}

void Camera::ToGameOverCameraWork()
{
}

void Camera::CameraShake(float x, float y, int time)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-x, x);
	std::uniform_real_distribution<float>dist2(-y, y);
	if (time > 0)
	{
		if (time % 2 != 1)
		{
			viewProjection_->eye_ = viewProjection_->eye_ + Vector3(dist(engine), dist2(engine), dist2(engine));

		}
		else
		{

		}
		time--;
	}
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
