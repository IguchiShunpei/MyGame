#pragma once

#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameClearScene.h"
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "SkyDome.h"
#include "LevelLoader.h"
#include "Meteor.h"
#include "Player.h"
#include "Enemy.h"
#include "InvEnemy.h"
#include "WeakEnemy.h"
#include "Sound.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>
#include <map>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

struct LevelData;

class CollisionManager;
class Player;

//�Q�[���v���C�V�[��
class GamePlayScene : public GameBaseScene
{
public:
	//�Q�[���V�[���̐؂�ւ�
	enum GameNum
	{
		FirstScene,
		BossScene,
	};
public:
	GamePlayScene();
	~GamePlayScene();

public:
	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//���
	void Finalize() override;

	//�Q�[���̃��Z�b�g
	void GameReset();

	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }
	const std::list<std::unique_ptr<InvEnemy>>& GetInvEnemys() { return invEnemys_; }

	const int GetBossNum() { return bossNum_; }

	//�G�f�[�^�ǂݍ���
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//BOSS��O�̉��o
	void BossAppears();

	//Clear��ʂւ̉��o
	void ToClearScene();

	//GameOver��ʂւ̉��o
	void ToGameOverScene();

	//�J�����V�F�C�N
	void CameraShake();


private://�����o�ϐ�
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
	InvEnemy* invEnemy;

	//�G
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	std::list<std::unique_ptr<InvEnemy>> invEnemys_;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	//�p�[�e�B�N��
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;

	Meteor* objMeteor = nullptr;

	Model* modelMeteor = nullptr;

	Meteor* meteor;

	//���x���f�[�^
	LevelData* levelData = nullptr;
	//���f��
	std::map<std::string, Model*> meteorModels;
	//�I�u�W�F�N�g
	std::vector<Meteor*> meteorObjects;

	//�V�[���ԍ�
	int gameNum;
	//wave�ԍ�
	int waveNum;

	//�J�������[�N�O�̍��W������ϐ�
	Vector3 cameraWorkPos_;
	//�{�X�̐�
	int bossNum_;
	//�ė����o�̒ė���
	float gameOverNum_;

	//�t���O
	//�{�X��ɓ�������
	bool isBossScene_;
	//�G�𔭐�������Ƃ��̑ҋ@�t���O
	bool isWait_;
	//�N���A���o�t���O
	bool isClearScene_;

	//�^�C�}�[
	//�ł��o���܂ł̎���
	float delayTimer_;
	//�{�X���o�̃^�C�}�[
	float bossAppTimer_;
	//�G�𔭐�������Ƃ��̑ҋ@����
	int waitTimer_;
	//Clear�Ɉڍs���鉉�o�̃^�C�}�[
	float clearTimer_;
	//�G�̍U���ɓ����������̉��o����
	int hitTimer_;
};
