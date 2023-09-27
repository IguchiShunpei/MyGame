#include "WeakEnemy.h"
#include "SphereCollider.h"
#include "GamePLayScene.h"
#include "string.h"

//�f�X�g���N�^
WeakEnemy::~WeakEnemy()
{
	delete wEnemyModel;
}

//������
void WeakEnemy::WEnemyInitialize()
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	wEnemyModel = Model::LoadFromOBJ("enemy02");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(wEnemyModel);
	isDead_ = false;
}

void WeakEnemy::Update()
{
	Move();

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
}

void WeakEnemy::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void WeakEnemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//���肪playerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isHit_ = true;
		hp_--;
		if (hp_ == 0)
		{
			isDead_ = true;
		}
	}
}

void WeakEnemy::Move()
{
	switch (phase_)
	{
	case Phase::None:
		break;
	case Phase::R:   //�J�[�u�t�F�[�Y
		RCurve();
		break;
	case Phase::L:   //�J�[�u�t�F�[�Y
		LCurve();
		break;
	}
}

void WeakEnemy::RCurve()
{
	MathFunc::CurveProjection(worldTransform_, startSpeed, C, flame);
}

void WeakEnemy::LCurve()
{
	MathFunc::CurveProjection(worldTransform_, { -startSpeed.x,startSpeed.y,startSpeed.z }, -C, flame);
}

Vector3 WeakEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
