/**
* @file Explosion.cpp
* @brief 爆発の処理クラス
* @author イグチ_シュンペイ
*/

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
		scaleNum_ = Vector3(0.2f, 0.2f, 0.2f);
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

	isFinish_ = false;

	alpha_ = 0.8f;
}

void Explosion::EnemyExplosionUpdate()
{
	worldTransform_.rotation_.y += 5.0f;
	worldTransform_.scale_ += scaleNum_;
	if (alpha_ >= 0.0f)
	{
		alpha_ -= 0.005f;
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
