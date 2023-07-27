#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include <list>

//GameScene�̑O���錾
class GamePlayScene;

class WeakEnemy : public Object3d
{
public:
	//�ړ��p�^�[��
	enum Phase
	{
		None,
		R,   //�E�J�[�u
		L,   //���J�[�u
	};

public:
	//�f�X�g���N�^
	~WeakEnemy();
	//������
	void WEnemyInitialize();

	//�X�V
	void Update();

	//�����蔻��X�V
	void ColliderUpdate();

	//setter
	//gameScene
	void SetGameScene(GamePlayScene* gameScene) { gameScene_ = gameScene; }

	//phase
	void SetPhase(Phase phase) { phase_ = phase; }

	bool GetIsDead() const { return isDead_; }

	bool GetIsDelete() const { return isDelete_; }

	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//�ړ�
	void Move();

	//�J�[�u
	void RCurve();
	void LCurve();

	//���[���h���W���擾
	Vector3 GetPosition();

private:
	//�Q�[���V�[��
	GamePlayScene* gameScene_ = nullptr;

	//�f�X�t���O
	//�|���ꂽ��
	bool isDead_ = false;
	//�f�X�|�[��������
	bool isDelete_ = false;

	// ���f��
	Model* wEnemyModel = nullptr;

	//�G�̈ړ��p�^�[��
	Phase phase_ = Phase::None;

	//�Ȃ���傫��
	const float C = 0.5f;
	//����
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//��������
	float flame = 0.0f;
};
