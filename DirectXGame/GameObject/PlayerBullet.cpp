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
	playerBulletModel_ = Model::LoadFromOBJ("triangle_mat");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(playerBulletModel_);
	SetScale(Vector3(3.5f, 3.5f, 3.5f));
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.position_ = position;
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	isDelete_ = false;
	chargeTime = 0;
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
	if (isCharge_ == false)
	{
		if (bulletNum_ == 0)
		{
			worldTransform_.UpdateMatrix();
		}
		else if (bulletNum_ == 1)
		{
			MathFunc::HorizontalProjection(worldTransform_, startSpeed, G, flame);
		}
		else if (bulletNum_ == 2)
		{
			isCharge_ = true;
			worldTransform_.scale_ += scaleNum;
		}

		if (worldTransform_.scale_.x >= 1.5f)
		{
			worldTransform_.scale_ -= Vector3(0.3f, 0.3f, 0.3f);
		}

		//���W���ړ�������
		worldTransform_.position_ += velocity_;

		//���Ԍo�߂Œe��������
		if (--deleteTimer_ <= 0)
		{
			isDelete_ = true;
		}
	}
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class Enemy";
	const char* str2 = "class WeakEnemy";
	const char* str3 = "class EnemyBullet";

	//���肪Enemy
	if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0 || strcmp(toCollisionName, str3) == 0) {
		isDelete_ = true;
	}
}
