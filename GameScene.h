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

private:
	//����
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//�J����
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//�v���C���[
	Player* player = nullptr;
	//�G
	Enemy* enemy = nullptr;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;
};
