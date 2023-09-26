#pragma once

#include "Sprite.h"

//�O���錾
class GameSceneManager;

//�V�[�����N���X
class GameBaseScene
{
public: //�����o�֐�
	GameBaseScene() = default;
	virtual ~GameBaseScene() = default;

public: // �����o�֐�
	
	//������
	virtual void Initialize() = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	//���
	virtual void Finalize() = 0;

public: //�����o�֐�
	virtual void SetSceneManager(GameSceneManager* gSceneManager) { gSceneManager_ = gSceneManager; }

private: //�����o�ϐ�
	//�V�[���}�l�[�W���i�؂�Ă���j
	GameSceneManager* gSceneManager_ = nullptr;
};

