/**
* @file Black.cpp
* @brief 黒画面の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"

class Black : public Object3d
{
public:
	~Black();

	//初期化
	void BlackInitialize();

	//更新
	void BlackUpdate();
	//フェードアウト(alpha値マイナス)
	void FadeOut();
	//フェードイン(alpha値プラス)
	void FadeIn();

	//描画
	void BlackDraw(ViewProjection* viewProjection_);

	//setter
	//isOut
	void SetIsOut(bool isOut) { this->isOut_ = isOut; }
	//isIn
	void SetIsIn(bool isIn){ this->isIn_ = isIn; }

	//getter
	//alpha
	float GetAlpha() const { return alpha_; }
	//isOut
	bool GetIsOut() const { return isOut_; }
	//isBlack
	bool GetIsBlack() const { return isBlack_; }
	//isFinish
	bool GetIsIn() const { return isIn_; }

private:
	// モデル
	Model* blackModel_ = nullptr;

	//alpha値
	float alpha_;
	//alpha最大値
	float alphaMax_;
	//alpha変動値
	float alphaNum_;
	//フェードアウトしたか
	bool isOut_;
	//黒画面になっているか
	bool isBlack_;
	//フェードインしたか
	bool isIn_;
};