#pragma once

#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"
#include "SkyDome.h"
#include "Sound.h"

#include <DirectXMath.h>

class GameClearScene : public GameBaseScene
{
public: //�����o�֐�
	//�R���X�g���N�^
	GameClearScene();
	//�f�X�g���N�^
	~GameClearScene();

public: //�����o�֐�
	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//���
	void Finalize() override;

private: // �����o�ϐ�
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SkyDome* sky;
	//�J����
	ViewProjection* viewProjection = nullptr;
};
