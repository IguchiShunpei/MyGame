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

	void Start();

	void Finish();

	//描画
	void BlackDraw(ViewProjection* viewProjection_);

	void SetIsStart(bool isStart) { this->isStart_ = isStart; }
	void SetIsFinish(bool isFinish){ this->isFinish_ = isFinish; }

	float GetAlpha() const { return alpha_; }
	bool GetIsStart() const { return isStart_; }
	bool GetIsBlack() const { return isBlack_; }
	bool GetIsFinish() const { return isFinish_; }

private:
	// モデル
	Model* blackModel_ = nullptr;

	float alpha_;
	bool isStart_;
	bool isBlack_;
	bool isFinish_;
};