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

	void Finish();

	void Reset();

	//描画
	void RedDraw(ViewProjection* viewProjection_);

	void SetIsRed(bool isRed){ this->isRed_ = isRed; }

private:
	// モデル
	Model* redModel_ = nullptr;

	float alpha_;
	bool isRed_;
};