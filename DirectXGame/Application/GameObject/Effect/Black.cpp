/**
* @file Black.cpp
* @brief 黒画面の処理クラス
* @author イグチ_シュンペイ
*/

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
	alphaMax_ = 1.0f;
	alphaNum_ = 0.02f;
	isBlack_ = true;
	isOut_ = true;
	isIn_ = false;
}

void Black::BlackUpdate()
{
	//フェードアウト
	FadeOut();
	//フェードイン
	FadeIn();
	//更新
	Update();
}

void Black::FadeOut()
{
	if (isOut_ == true && isBlack_ == true)
	{
		//alphaが0以上ならマイナス
		if (alpha_ >= 0.0f)
		{
			alpha_ -= alphaNum_;
		}
		else
		{
			alpha_ = 0.0f;
			isBlack_ = false;
			isOut_ = false;
		}
	}
}

void Black::FadeIn()
{
	if (isIn_ == true && isBlack_ == false)
	{
		//alphaが1以下ならプラス
		if (alpha_ <= alphaMax_)
		{
			alpha_ += alphaNum_;
		}
		else
		{
			alpha_ = alphaMax_;
			isBlack_ = true;
			isIn_ = false;
		}
	}
}

void Black::BlackDraw(ViewProjection* viewProjection_)
{
	Draw(viewProjection_, alpha_, { 1.0f,1.0f,1.0f });
}
