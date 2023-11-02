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

	void SetIsFinish(bool isFinish){ this->isFinish_ = isFinish; }

private:
	// モデル
	Model* blackModel_ = nullptr;

	float alpha_;
	bool isStart_;
	bool isBlack_;
	bool isFinish_;
};