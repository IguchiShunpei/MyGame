#pragma once
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "WeakEnemy.h"
#include "ParticleManager.h"
#include "SkyDome.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class CollisionManager;
class Player;

class GameScene : public SIFrameWork
{
public:
	enum SceneNum
	{
		Title,
		Game,
		Clear,
		GameOver,
		Pose,
	};

public:
	//������
	void Initialize() override;
	//�I��
	void Finalize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;

	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }

	//�G�f�[�^�ǂݍ���
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//BOSS��O�̉��o
	void BossAppears();

private:
	//�w�i
	SkyDome* sky;
	//����
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//�J����
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//�v���C���[
	Player* player;

	//�G
	Enemy* enemy;
	WeakEnemy* wEnemy;

	//�G
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	
	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	//�p�[�e�B�N��
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;

	//�V�[���ԍ�
	int sceneNum;
	//wave�ԍ�
	int waveNum;
	//�{�X��ɓ�������
	bool isBossScene;
	//�{�X���o�̃^�C�}�[
	int bossAppTimer;
	//�{�X��̃J�������[�N�O�̍��W������ϐ�
	Vector3 bossCameraPos;

	//�ł��o���܂ł̎���
	float delayTimer = 0.0f;

	bool isWait_ = false;

	int waitTimer = 10;
};
