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

	viewProjection->Initialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//���̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//���̏�����
	newEnemy->EnemyInitialize();
	//���̓o�^
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;

	//�J�������
	delete viewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//���͂̍X�V
	input->Update();

	//�f�X�t���O�̗������e���폜
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});

	if (input->PushKey(DIK_UP))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, -0.01f, 0));
	}
	if (input->PushKey(DIK_RIGHT))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(-0.01f, 0, 0));
	}
	if (input->PushKey(DIK_LEFT))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0.01f, 0, 0));
	}
	if (input->PushKey(DIK_DOWN))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0.01f, 0));
	}

	//�ړ����E���W
	const float kCameraLimitX = 0.28f;
	const float kCameraLimitY = 0.2f;

	//�͈͂𒴂��Ȃ�����
	viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
	viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
	viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
	viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

	//�J����
	viewProjection->UpdateMatrix();
	//�v���C���[
	player->Update();
	//�G
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
		enemy->ColliderUpdate();
	}

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
	//�G
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw(viewProjection);
		enemy->BulletDraw(viewProjection);
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
