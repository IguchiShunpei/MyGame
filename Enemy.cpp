#include "Enemy.h"
#include "SphereCollider.h"
#include "GameScene.h"
#include "string.h"

//�f�X�g���N�^
Enemy::~Enemy() {
	delete enemyModel;
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
	SetPosition(Vector3(0, 0, 100));
	isDead_ = false;
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//���肪playerBullet
	if (strcmp(toCollisionName, str1) == 0) {
		isDead_ = true;
	}
}