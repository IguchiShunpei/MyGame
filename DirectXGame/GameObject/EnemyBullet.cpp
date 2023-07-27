#include "EnemyBullet.h"
#include "SphereCollider.h"

EnemyBullet::~EnemyBullet()
{
	delete enemyBulletModel_;
}

void EnemyBullet::EnemyBulletInitialize(const Vector3& position, const Vector3& velocity)
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	enemyBulletModel_ = Model::LoadFromOBJ("triangle_mat");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(enemyBulletModel_);
	SetScale(Vector3(1.5f, 1.5f, 1.5f));
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.position_ = position;
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	isDelete_ = false;
}

void EnemyBullet::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}


void EnemyBullet::Update()
{
	worldTransform_.UpdateMatrix();

	//���W���ړ�������
	worldTransform_.position_ -= velocity_;

	//���Ԍo�߂Œe��������
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void EnemyBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Player";
	const char* str2 = "class PlayerBullet";

	//���肪Enemy
	if (strcmp(toCollisionName, str1) == 0|| strcmp(toCollisionName, str2) == 0) {
		isDelete_ = true;
	}
}
