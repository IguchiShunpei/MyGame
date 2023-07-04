#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

void GameScene::Initialize()
{
	SIFrameWork::Initialize();

	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//�����蔻��
	collisionManager = CollisionManager::GetInstance();

	//�J����������
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//enemy
	enemy = new Enemy;
	enemy->EnemyInitialize();
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;

	//�J�������
	delete viewProjection;
	delete xmViewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//���͂̍X�V
	input->Update();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
	{
		// ���݂̍��W���擾
		Vector3 cameraPos = viewProjection->GetEye();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_W)) { cameraPos.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { cameraPos.y -= 0.1f; }
		if (input->PushKey(DIK_A)) { cameraPos.x += 0.1f; }
		else if (input->PushKey(DIK_D)) { cameraPos.x -= 0.1f; }

		// ���W�̕ύX�𔽉f
		viewProjection->SetEye(cameraPos);
	}

	//�J����
	viewProjection->UpdateMatrix();
	xmViewProjection->Update();

	player->Update();
	enemy->Update();
	enemy->ColliderUpdate();

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
	 
	Object3d::PreDraw(dxCommon->GetCommandList());

	player->Draw(viewProjection);
	player->BulletDraw(viewProjection);
	if (enemy->GetIsDead() == false)
	{
		enemy->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//�G�t�F�N�g�`��O����
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();

	//�����Ƀ|���S���Ȃǂ̕`�揈��������
	/*sprite_1->Draw(dxCommon);
	sprite_2->Draw(dxCommon);*/

#pragma endregion �ŏ��̃V�[���̕`��

	// �`��㏈��
	dxCommon->PostDraw();
}
