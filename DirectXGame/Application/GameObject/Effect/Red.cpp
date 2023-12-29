/**
* @file Red.cpp
* @brief 赤画面の処理クラス
* @author イグチ_シュンペイ
*/

#include "Red.h"

Red::~Red()
{
	delete redModel_;
}

void Red::RedInitialize()
{
	//初期化
	Initialize();

	// OBJからモデルデータを読み込む
	redModel_ = Model::LoadFromOBJ("Red");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(redModel_);
	SetScale(Vector3(100.0f, 100.0f, 0.01f));

	alpha_ = 0.5f;
	alphaMax_ = 0.5f;
	alphaNum_ = 0.02f;
	isRed_ = false;
}

void Red::RedUpdate()
{
	FadeOut();
	Update();
}

void Red::FadeOut()
{
	if (isRed_ == true)
	{
		//alphaが0以上ならマイナス
		if (alpha_ >= 0.0f)
		{
			alpha_ -= alphaNum_;
		}
		else
		{
			isRed_ = false;
		}
	}
}

void Red::Reset()
{
	//alpha値リセット
	alpha_ = alphaMax_;
}

void Red::RedDraw(ViewProjection* viewProjection_)
{
	if (isRed_ == true)
	{
		Draw(viewProjection_, alpha_, { 1.0f,1.0f,1.0f });
	}
}
