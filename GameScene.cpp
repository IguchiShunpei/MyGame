#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <sstream>

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
	viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	LoadEnemy();

	//�p�[�e�B�N��
	p_dmg = Particle::LoadParticleTexture("effect1.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;
	delete p_dmg;
	delete pm_dmg;

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

	//�f�X�t���O�̗������G���폜
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
	const float kCameraLimitX = 0.28f * 1.7f;
	const float kCameraLimitY = 0.2f * 1.7f;

	//�͈͂𒴂��Ȃ�����
	viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
	viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
	viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
	viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

	//�J����
	viewProjection->UpdateMatrix();

	//�V��
	sky->Update();

	//�v���C���[
	player->Update();
	player->ColliderUpdate();
	//�G
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
		enemy->ColliderUpdate();
	}
	

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		if (enemy->GetIsDead() == true)
		{
			pm_dmg->Fire(p_dmg, 50, { enemy->GetWorldPosition().x,enemy->GetWorldPosition().y,enemy->GetWorldPosition().z
				}, 30.0f, 30.0f, 30.0f, 30.0f, 0, 0, 0, 0, 0.2f, 0.2f, 0, 0, 0, 3, { 4.0f, 0.0f });
		}
	}

	pm_dmg->Update();
}

void GameScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);
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

	pm_dmg->Draw();

	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();

	//�����Ƀ|���S���Ȃǂ̕`�揈��������
	/*sprite_1->Draw(dxCommon);
	sprite_2->Draw(dxCommon);*/

#pragma endregion �ŏ��̃V�[���̕`��

	// �`��㏈��
	dxCommon->PostDraw();
}

void GameScene::LoadEnemy()
{
	enemys_.clear();

	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	HRESULT result = S_FALSE;

	std::string num;

	// �P�s���ǂݍ���
	std::string line;
	while (getline(file, line)) {

		// �P�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�p�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// �擪�����񂪂��Ȃ璸�_���W
		if (key == "enemy") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy->SetPosition(position);
			//�o�^
			enemys_.push_back(std::move(newEnemy));
		}
	}
	// �t�@�C���ƕ���
	file.close();
}
