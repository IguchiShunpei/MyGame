#include "Explosion.h"
#include "GamePlayScene.h"

Explosion::~Explosion()
{
	delete explosionModel01_;
	delete explosionModel02_;
	delete explosionModel03_;
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
	else if (num == 1)
	{
		// OBJからモデルデータを読み込む
		explosionModel02_ = Model::LoadFromOBJ("explosion02");
		// 3Dオブジェクト生成
		Create();
		// オブジェクトにモデルをひも付ける
		SetModel(explosionModel02_);
		scaleNum_ = Vector3(0.1f, 0.1f, 0.1f);
		SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
	else if (num == 2)
	{
		// OBJからモデルデータを読み込む
		explosionModel03_ = Model::LoadFromOBJ("explosion03");
		// 3Dオブジェクト生成
		Create();
		// オブジェクトにモデルをひも付ける
		SetModel(explosionModel03_);
		scaleNum_ = Vector3(0.5f, 0.5f, 0.5f);
		SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
	SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	color_ = Vector3(1.0f, 1.0f, 1.0f);

	updateTimer_ = 0;

	isFinish_ = false;

	alpha_ = 0.8f;
}

void Explosion::EnemyExplosionUpdate(Vector3 bossDeadPos)
{
	if (updateTimer_ % 2 != 1)
	{
		//乱数生成装置
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());
		std::uniform_real_distribution<float>dist(-3.0, 3.0);
		std::uniform_real_distribution<float>dist2(-3.0, 3.0);

		worldTransform_.position_ = Vector3(dist(engine), dist2(engine), bossDeadPos.z);
	}
	else
	{
		SetPosition(bossDeadPos);
	}
	updateTimer_++;
	worldTransform_.scale_ += scaleNum_;
	if (alpha_ >= 0.0f)
	{
		alpha_ -= 0.0045f;
	}
	else
	{
		isFinish_ = true;
	}
	Update();
}

void Explosion::PlayerExplosionUpdate(Vector3 playerDeadpos)
{
	SetPosition(playerDeadpos);
	if (isFinish_ == false)
	{
		if (worldTransform_.scale_.x <= 50.0f)
		{
			worldTransform_.scale_ += scaleNum_;
			Update();
		}
		else
		{
			if (alpha_ >= 0.0f)
			{
				alpha_ -= 0.025f;
			}
			else
			{
				isFinish_ = true;
			}
		}
	}
}

void Explosion::ExplosionDraw(ViewProjection* viewProjection_)
{
	Draw(viewProjection_, alpha_, color_);
}
