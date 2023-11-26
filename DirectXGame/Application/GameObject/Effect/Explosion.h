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

	void ExplosionUpdate(Vector3 bossDeadpos);

	void ExplosionDraw(ViewProjection* viewProjection_);

	bool GetIsFinish() const { return isFinish_; }

private:

	// モデル
	Model* explosionModel01_ = nullptr;
	Model* explosionModel02_ = nullptr;

	Vector3 scaleNum_;

	float alpha_;

	int updateTimer_;

	bool isFinish_;

	Vector3 color_;
};