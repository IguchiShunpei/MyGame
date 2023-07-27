#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//GameScene�̑O���錾
class GamePlayScene;

class Enemy : public Object3d
{
	//�ړ��p�^�[��
	enum class Phase
	{
		None,
		Approach,//�ڋ�
		Leave,   //���E
		Curve,   //�J�[�u
		ReCurve, //�t�����J�[�u
	};

public:
	//�f�X�g���N�^
	~Enemy();
	//������
	void EnemyInitialize();

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

	//�U��
	void Attack();
	
	//�ړ�
	void Move();

	//�ڋ�
	void Approach();
	//���E
	void Leave();
	//�J�[�u
	void Curve();
	void ReCurve();

	//�e��`��
	void BulletDraw(ViewProjection* viewProjection_);

	//�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//phaseNum��setter
	void SetPhaseNum(int phaseNum) { this->phaseNum_ = phaseNum; }

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
	Model* enemyModel = nullptr;

	//�e
	EnemyBullet* enemyBullet;

	//�e���X�g
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//�e��ł��o���܂ł̎���
	float dalayTimer = 5.0f;

	//�s���p�^�[��
	int phaseNum_ = 0;

	//�ڋߑ��x
	Vector3 approach_ = { 0.0f,0.0f,-0.3f };

	//���E���x
	Vector3 leave_ = { 0.0f,0.0f,1.0f };
	
	//�G�̈ړ��p�^�[��
	Phase phase_ = Phase::None;

	//�Ȃ���傫��
	const float C = 0.5f;
	//����
	Vector3 startSpeed = { -0.5f,0.0f,-0.5f };
	//��������
	float flame = 0.0f;
};
