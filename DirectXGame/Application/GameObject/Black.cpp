#include "Black.h"

Black::~Black()
{
	delete blackModel_;
}

void Black::BlackInitialize()
{
	Initialize();

	// OBJからモデルデータを読み込む
	blackModel_ = Model::LoadFromOBJ("Black");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(blackModel_);
	SetScale(Vector3(100.0f, 100.0f, 0.01f));

	alpha_ = 1.0f;
	isBlack_ = true;
	isStart_ = true;
	isFinish_ = false;
}

void Black::BlackUpdate()
{
	Start();
	Finish();
	Update();
}

void Black::Start()
{
	if (isStart_ == true && isBlack_ == true)
	{
		if (alpha_ >= 0.0f)
		{
			alpha_ -= 0.02f;
		}
		else
		{
			alpha_ = 0.0f;
			isBlack_ = false;
			isStart_ = false;
		}
	}
}

void Black::Finish()
{
	if (isFinish_ == true && isBlack_ == false)
	{
		if (alpha_ <= 1.0f)
		{
			alpha_ += 0.02f;
		}
		else
		{
			alpha_ = 1.0f;
			isBlack_ = true;
		}
	}
}

void Black::BlackDraw(ViewProjection* viewProjection_)
{
	Draw(viewProjection_, alpha_, { 1.0f,1.0f,1.0f });
}
