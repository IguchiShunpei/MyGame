#include "Explosion.h"

Explosion::~Explosion()
{
	delete explosionModel01_;
	delete explosionModel02_;
}

void Explosion::ExplosionInitialize(int num)
{
	Initialize();

	if (num == 0)
	{
		// OBJからモデルデータを読み込む
		explosionModel01_ = Model::LoadFromOBJ("explosion01");
		// 3Dオブジェクト生成
		Create();
		// オブジェクトにモデルをひも付ける
		SetModel(explosionModel01_);
		scaleNum_ = Vector3(0.3f, 0.3f, 0.3f);
		SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
	else if(num == 1)
	{
		// OBJからモデルデータを読み込む
		explosionModel02_ = Model::LoadFromOBJ("explosion02");
		// 3Dオブジェクト生成
		Create();
		// オブジェクトにモデルをひも付ける
		SetModel(explosionModel02_);
		scaleNum_ = Vector3(0.2f, 0.2f, 0.2f);
		SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
	SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	color_ = Vector3(1.0f, 1.0f, 1.0f);

	alpha_ = 0.8f;
}

void Explosion::Update()
{
	worldTransform_.scale_ += scaleNum_;
	if (alpha_ >= 0.0f)
	{
		alpha_ -= 0.0045f;
	}
	worldTransform_.UpdateMatrix();
}

void Explosion::ExplosionDraw(ViewProjection* viewProjection_)
{
	Draw(viewProjection_, alpha_, color_);
}
