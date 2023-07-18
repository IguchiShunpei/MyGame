#pragma once
#include "Object3d.h"
#include "Model.h"
#include "MathFunc.h"

class PlayerBullet : public Object3d
{
public:
	//�f�X�g���N�^
	~PlayerBullet();
	//������
	void PlayerBulletInitialize(const Vector3& position, const Vector3& velocity);
	//�����蔻��X�V
	void ColliderUpdate();

	void Update();

	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//getter
	bool GetIsDelete() const { return isDelete_; }
	bool GetIsCharge() const { return isCharge_; }

	//bulletNum��setter
	void SetBulletNum(int bulletNum) { this->bulletNum_ = bulletNum; }

private:

	// ���f��
	Model* playerBulletModel_ = nullptr;
	//�폜�t���O
	bool isDelete_ = false;
	//�e���
	int bulletNum_ = 0;
	//�폜�^�C�}�[
	float deleteTimer_ = 150.0f;
	//���x
	Vector3 velocity_;

	//�d��
	const float G = 0.3f;
	//����
	Vector3 startSpeed = { 0.0f,0.0f,0.0f };
	//��������
	float flame = 0.0f;

	//���ߎ���
	float chargeTime;
	//���߂Ă��邩
	bool isCharge_ = false;
	//�g�吔�l
	Vector3 scaleNum;
};