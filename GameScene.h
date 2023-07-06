#pragma once
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "ParticleManager.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class CollisionManager;
class Player;

class GameScene : public SIFrameWork
{
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

private:
	//�w�i
	Sprite* sprite_01;
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

	//�G
	std::list<std::unique_ptr<Enemy>> enemys_;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;
};
