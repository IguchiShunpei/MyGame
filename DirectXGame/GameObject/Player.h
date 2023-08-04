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

//GameScene�̑O���錾
class GamePlayScene;

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

	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

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

	//�e�X�V
	void BulletUpdate();

	//�e�`��
	void BulletDraw(ViewProjection* viewProjection_);

	//�e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//getter
	//IsInit
	bool GetIsInit() const { return isInit_; }
	//IsHit
	bool GetIsHit() const { return isHit_; }

	//setter
	//IsHit
	void SetIsHit(bool isHit) { isHit_ = isHit; }

private:
	//����
	Input* input_ = nullptr;
	// ���f��
	Model* playerModel_ = nullptr;

	//�e
	PlayerBullet* playerBullet_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�e���
	int bulletNum_;

	//�^�C�}�[
	//�ł��o���܂ł̎���
	float dalayTimer_;
	//�o�ꃂ�[�V�����̎���
	float initMotionTime_;

	//�t���O
	//�|������
	bool isBurst_;
	//�o�ꂵ����
	bool isInit_;
	//�G�̍U���ɓ���������
	bool isHit_;

	//�ړ����̌X��
	bool isRightRotation_;
	bool isLeftRotation_;
	bool isUpRotation_;
	bool isDownRotation_;
};
