#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "MathFunc.h"
#include <memory>
#include <list>

class Player : public Object3d
{
public:
	//�f�X�g���N�^
	~Player();
	//������
	void PlayerInitialize();

	//�X�V
	void Update();

	//�����蔻��X�V
	void ColliderUpdate();

	//�o�ꃂ�[�V����
	void IntitMotion();

	//�ړ�
	void Move();

	//�X��
	void Rotate();

	//�U��
	void Attack();

	//�e�؂�ւ�
	void ChangeBullet();

	//�e�`��
	void BulletDraw(ViewProjection* viewProjection_);

	//�e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//getter
	bool GetIsInit() const { return isInit; }

private:
	//����
	Input* input = nullptr;
	// ���f��
	Model* playerModel = nullptr;

	//�e���
	int bulletNum = 0;

	//�e
	PlayerBullet* playerBullet;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�ł��o���܂ł̎���
	float dalayTimer = 0.0f;

	//�o�ꃂ�[�V�����̎���
	float initMotionTime;

	//�U��
	bool isBurst = false;

	bool isInit = false;

	//�ړ����̌X��
	bool isRightRotation;
	bool isLeftRotation;
	bool isUpRotation;
	bool isDownRotation;
};
