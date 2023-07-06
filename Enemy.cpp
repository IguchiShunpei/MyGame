#include "Enemy.h"
#include "SphereCollider.h"
#include "GameScene.h"
#include "string.h"

//�f�X�g���N�^
Enemy::~Enemy() {
	delete enemyModel;
	delete enemyBullet;
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
	SetPosition(Vector3(0, 0,100));
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
	isDead_ = false;
}

void Enemy::Update()
{
	Attack();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

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
	if (strcmp(toCollisionName, str1) == 0) {
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
		newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 2.0f));

		//���̓o�^
		bullets_.push_back(std::move(newBullet));

		dalayTimer = 5.0f;
	}
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
