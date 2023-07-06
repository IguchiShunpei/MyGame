#pragma once
#include "Object3d.h"
#include "Model.h"

class EnemyBullet : public Object3d
{
public:
	//�f�X�g���N�^
	~EnemyBullet();
	//������
	void EnemyBulletInitialize(const Vector3& position, const Vector3& velocity);
	//�����蔻��X�V
	void ColliderUpdate();

	void Update();

	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//isDead��getter
	bool GetIsDelete() const { return isDelete_; }

private:

	// ���f��
	Model* enemyBulletModel_ = nullptr;
	//�폜�t���O
	bool isDelete_ = false;
	//�폜�^�C�}�[
	float deleteTimer_ = 200.0f;
	//���x
	Vector3 velocity_;
};