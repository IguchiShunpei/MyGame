#include "InvEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//�f�X�g���N�^
InvEnemy::~InvEnemy()
{
	delete enemyModel;
}

//������
void InvEnemy::InvEnemyInitialize()
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	enemyModel = Model::LoadFromOBJ("enemy03");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(enemyModel);
	isDelete_ = false;
	isHit_ = false;
}

void InvEnemy::Update()
{
	if (isTurn_ == false && isStart_ == false)
	{
		waitTimer++;
		if (waitTimer >= 60)
		{
			isTurn_ = true;
		}
	}
	Turn();
	Move();

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
}

void InvEnemy::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void InvEnemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//���肪playerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{

	}
}

void InvEnemy::Move()
{
	if (isStart_ == true)
	{
		worldTransform_.position_.z -= 3.0f;
		if (worldTransform_.position_.z <= -20.0f)
		{
			isDelete_ = true;
		}
	}
}

void InvEnemy::Turn()
{
	if (isTurn_ == true)
	{
		turnTimer_++;
		worldTransform_.rotation_.z = 360.0f * MathFunc::easeOutSine(turnTimer_ / 30.0f);
		if (turnTimer_ >= 30.0f)
		{
			worldTransform_.rotation_.z = 360.0f;
			isTurn_ = false;
			isStart_ = true;
		}
	}
}


Vector3 InvEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
