/**
* @file Red.cpp
* @brief 赤画面の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"

class Red : public Object3d
{
public:
	~Red();

	//初期化
	void RedInitialize();

	//更新
	void RedUpdate();

	//フェードアウト(alpha値マイナス)
	void FadeOut();

	//リセット
	void Reset();

	//描画
	void RedDraw(ViewProjection* viewProjection_);

	//setter
	//isRed
	void SetIsRed(bool isRed){ this->isRed_ = isRed; }
	//isOut
	void SetIsOut(bool isOut) { this->isOut_ = isOut; }

private:
	// モデル
	Model* redModel_ = nullptr;

	//alpha値
	float alpha_;
	//alpha最大値
	float alphaMax_;
	//alpha変動値
	float alphaNum_;
	//赤画面になったか
	bool isRed_;
	//フェードアウトしたか
	bool isOut_;
};