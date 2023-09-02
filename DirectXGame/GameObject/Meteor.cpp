#include "Meteor.h"
#include "GameScene.h"

void Meteor::MeteorInitialize()
{
	//初期化
	Initialize();
	// 3Dオブジェクト生成
	Create();
}

void Meteor::MeteorUpdate()
{
	Move();

	Update();
}

void Meteor::Move()
{
	worldTransform_.position_.z -= 0.05f;
	if (worldTransform_.position_.z <= -20.0f)
	{
		worldTransform_.position_.z = 70.0f;
	}
}
