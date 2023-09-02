#include "Meteor.h"
#include "GameScene.h"

void Meteor::MeteorInitialize()
{
	//������
	Initialize();
	// 3D�I�u�W�F�N�g����
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
