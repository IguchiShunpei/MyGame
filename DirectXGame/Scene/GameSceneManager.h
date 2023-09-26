#pragma once

#include "GameBaseScene.h"
#include "AbstractSceneFactory.h"

//�V�[���Ǘ�
class GameSceneManager final
{
private: //�����o�֐�
	//�R���X�g���N�^
	GameSceneManager() = default;
	//�f�X�g���N�^
	~GameSceneManager();

public: //�����o�֐�
	//�X�V
	void Update();

	//�`��
	void Draw();

	//�V�[���̉��
	void Destroy();

public:
	//�C���X�^���X�̍쐬
	static GameSceneManager* GetInstance();

private:
	//�R�s�[�R���X�g���N�^�𖳌��ɂ���
	GameSceneManager(const GameSceneManager&) = delete;
	//������Z�q�𖳌��ɂ���
	GameSceneManager& operator=(const GameSceneManager&) = delete;

public: //�����o�֐�
	//���V�[���\��
	void ChangeScene(const std::string& sceneName);

	//�V�[���t�@�N�g���[��setter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private: //�����o�ϐ�
	//���̃V�[��(���s���V�[��)
	GameBaseScene* scene_ = nullptr;
	//���V�[��
	GameBaseScene* nextScene_ = nullptr;
	//�V�[���t�@�N�g���[�i�؂�Ă���j
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

