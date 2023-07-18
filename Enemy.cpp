#include "Enemy.h"
#include "SphereCollider.h"
#include "GameScene.h"
#include "string.h"

//�f�X�g���N�^
Enemy::~Enemy() 
{
	delete enemyModel;
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		delete enemyBullet;
	}
}

//������
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	enemyModel = Model::LoadFromOBJ("enemy");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(enemyModel);
	isDead_ = false;
}

void Enemy::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	Move();

	Attack();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
}

void Enemy::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//���肪playerBullet
	if (strcmp(toCollisionName, str1) == 0) 
	{
		isDead_ = true;
	}
}

void Enemy::Attack()
{
	dalayTimer -= 0.1f;

	//���L�����̍��W���R�s�[
	Vector3 position = GetWorldPosition();

	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//�N�[���^�C�����O�ɂȂ����Ƃ�
	if (dalayTimer <= 0)
	{
		//���̐���
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//���̏�����
		newBullet->EnemyBulletInitialize(position, velocity);

		//�R���C�_�[�̒ǉ�
		newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

		//���̓o�^
		bullets_.push_back(std::move(newBullet));

		dalayTimer = 5.0f;
	}
}

void Enemy::Move()
{
	switch (phase_)
	{
	case Phase::Approach: //�ڋ߃t�F�[�Y
	default:
		//�ړ�(�x�N�g�������Z)
		Approach();
		break;
	case Phase::Leave:   //���E�t�F�[�Y
		Leave();
		break;

	}
}

void Enemy::Approach()
{
	worldTransform_.position_ += approach_;

	//����̈ʒu�ɒ������痣�E��
	if (worldTransform_.position_.z <= 30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	worldTransform_.position_ += leave_;

	//����̈ʒu�ɒ�������ڋ߂�
	if (worldTransform_.position_.z >= 80.0f)
	{
		phase_ = Phase::Approach;
	}
}

void Enemy::Curve()
{

}

void Enemy::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
