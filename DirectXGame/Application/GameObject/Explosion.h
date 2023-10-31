#pragma once
#include "Object3d.h"
#include "Model.h"

class Explosion : public Object3d
{
public:
	//デストラクタ
	~Explosion();
	//初期化
	void ExplosionInitialize(int num);

	void Update();

	void ExplosionDraw(ViewProjection* viewProjection_);

private:

	// モデル
	Model* explosionModel01_ = nullptr;
	Model* explosionModel02_ = nullptr;

	Vector3 scaleNum_;

	float alpha_;

	Vector3 color_;
};