#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//GameScene�̑O���錾
class GameScene;

class InvEnemy : public Object3d
{
public:
	//�f�X�g���N�^
	~InvEnemy();
	//������
	void InvEnemyInitialize();

	//�X�V
	void Update();

	//�����蔻��X�V
	void ColliderUpdate();

	//getter
	//IsDelete
	bool GetIsDelete() const { return isDelete_; }
	
	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//�U��
	void Attack();
	
	//�ړ�
	void Move();

	//Setter
	//gameScene
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	//���[���h���W���擾
	Vector3 GetPosition();

private:
	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	//�f�X�t���O
	//�f�X�|�[��������
	bool isDelete_ = false;

	// ���f��
	Model* enemyModel = nullptr;
	
	int waitTimer = 0;
};
