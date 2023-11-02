#include "Explosion.h"
#include "GamePlayScene.h"

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
		scaleNum_ = Vector3(0.1f, 0.1f, 0.1f);
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
		scaleNum_ = Vector3(0.05f, 0.05f, 0.05f);
		SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
	SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	color_ = Vector3(1.0f, 1.0f, 1.0f);

	alpha_ = 0.8f;
}

void Explosion::ExplosionUpdate()
{
	worldTransform_.position_ = { 0.0f,0.0f,0.0f };
	worldTransform_.UpdateMatrix();

	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-1.0, 1.0);
	std::uniform_real_distribution<float>dist2(-1.0, 1.0);

	worldTransform_.position_ += Vector3(dist(engine), dist2(engine), dist2(engine));

	worldTransform_.scale_ += scaleNum_;
	if (alpha_ >= 0.0f)
	{
		alpha_ -= 0.0045f;
	}
	Update();
}

void Explosion::ExplosionDraw(ViewProjection* viewProjection_)
{
	Draw(viewProjection_, alpha_, color_);
}
