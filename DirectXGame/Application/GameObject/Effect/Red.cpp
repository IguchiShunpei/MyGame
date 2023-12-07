#include "Red.h"

Red::~Red()
{
	delete redModel_;
}

void Red::RedInitialize()
{
	Initialize();

	// OBJからモデルデータを読み込む
	redModel_ = Model::LoadFromOBJ("Red");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(redModel_);
	SetScale(Vector3(100.0f, 100.0f, 0.01f));

	alpha_ = 0.5f;
	isRed_ = false;
}

void Red::RedUpdate()
{
	Finish();
	Update();
}

void Red::Finish()
{
	if (isRed_ == true)
	{
		if (alpha_ >= 0.0f)
		{
			alpha_ -= 0.02f;
		}
		else
		{
			isRed_ = false;
			alpha_ = 0.0f;
		}
	}
}

void Red::Reset()
{
	alpha_ = 0.5f;
}

void Red::RedDraw(ViewProjection* viewProjection_)
{
	if (isRed_ == true)
	{
		Draw(viewProjection_, alpha_, { 1.0f,1.0f,1.0f });
	}
}
