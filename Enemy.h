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
public:
	//�f�X�g���N�^
	~Enemy();
	//������
	void EnemyInitialize();

	//�X�V
	void Update();

	//�����蔻��X�V
	void ColliderUpdate();

	//gameScene��setter
	void SetGameScene(GamePlayScene* gameScene) { gameScene_ = gameScene; }

	bool GetIsDead() const { return isDead_; }
	
	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//�U��
	void Attack();

	//�e��`��
	void BulletDraw(ViewProjection* viewProjection_);

	//�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

private:
	//�Q�[���V�[��
	GamePlayScene* gameScene_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	// ���f��
	Model* enemyModel = nullptr;

	//�e
	EnemyBullet* enemyBullet;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//�ł��o���܂ł̎���
	float dalayTimer = 5.0f;

	//���a
	float radius = 1.0f;
};
