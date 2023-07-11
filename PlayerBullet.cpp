#include "PlayerBullet.h"
#include "SphereCollider.h"

PlayerBullet::~PlayerBullet()
{
	delete playerBulletModel_;
}

void PlayerBullet::PlayerBulletInitialize(const Vector3& position, const Vector3& velocity)
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	playerBulletModel_ = Model::LoadFromOBJ("ironSphere_01");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(playerBulletModel_);
	SetScale(Vector3(0.25f, 0.25f, 0.25f));
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.position_ = position;
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	isDelete_ = false;
	bulletNum = 0;
}

void PlayerBullet::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}


void PlayerBullet::Update()
{
	worldTransform_.UpdateMatrix();
	/*MathFunc::HorizontalProjection(worldTransform_, startSpeed, G, flame);*/

	//���W���ړ�������
	worldTransform_.position_ += velocity_;

	//���Ԍo�߂Œe��������
	if (--deleteTimer_ <= 0)
	{
		isDelete_ = true;
	}
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Enemy";
	const char* str2 = "class EnemyBullet";

	//���肪Enemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0) {
		isDelete_ = true;
	}
}
