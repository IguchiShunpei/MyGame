#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>

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

	LoadEnemyPop();

	//�p�[�e�B�N��
	p_dmg = Particle::LoadParticleTexture("effect1.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	//�����o�ϐ��̏�����
	bossCameraPos = { 0,0,0 };

	//�t���O
	isBossScene = false;
	sceneNum = 0;
	bossAppTimer = 0;
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;
	delete wEnemy;
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

	//�f�X�t���O�̗������G���폜
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDead();
		});

	//�V��
	sky->Update();

	player->IntitMotion();

	BossAppears();

	//�J����
	viewProjection->UpdateMatrix();

	if (player->GetIsInit() == true)
	{
		//�X�V�R�}���h
		UpdateEnemyPop();

		if (isBossScene == false)
		{
			if (input->PushKey(DIK_UP))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, -0.05f, 0));
			}
			if (input->PushKey(DIK_RIGHT))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(-0.05f, 0, 0));
			}
			if (input->PushKey(DIK_LEFT))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0.05f, 0, 0));
			}
			if (input->PushKey(DIK_DOWN))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0.05f, 0));
			}

			//�ړ����E���W
			const float kCameraLimitX = 1.5f * 1.7f;
			const float kCameraLimitY = 1.1f * 1.7f;

			//�͈͂𒴂��Ȃ�����
			viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
			viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
			viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
			viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

			//�v���C���[
			player->Update();
			player->ColliderUpdate();

			//�G
			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				enemys->Update();
				enemys->ColliderUpdate();
			}
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Update();
				wEnemys->ColliderUpdate();
			}

			//�S�Ă̏Փ˂��`�F�b�N
			collisionManager->CheckAllCollisions();

			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				if (enemys->GetIsDead() == true)
				{
					Vector3 deadPos{};
					deadPos = enemys->GetPosition();
					pm_dmg->Fire(p_dmg, 50,
						{ deadPos.x,deadPos.y,deadPos.z },
						7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 8, { 4.0f, 0.0f });
				}
			}

			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				if (wEnemys->GetIsDead() == true)
				{
					Vector3 deadPos{};
					deadPos = wEnemys->GetPosition();
					pm_dmg->Fire(p_dmg, 50,
						{ deadPos.x,deadPos.y,deadPos.z },
						7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 8, { 4.0f, 0.0f });
				}
			}
			pm_dmg->Update();
		}
		else
		{
			player->BulletUpdate();
		}
	}
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
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		enemys->Draw(viewProjection);
		enemys->BulletDraw(viewProjection);
	}

	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		wEnemys->Draw(viewProjection);
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

void GameScene::LoadEnemyPop()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	// �t�@�C���ƕ���
	file.close();
}

void GameScene::UpdateEnemyPop()
{
	//�ҋ@����
	if (isWait_ == true)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//�ҋ@����
			isWait_ = false;
		}
		return;
	}

	// �P�s���ǂݍ���
	std::string line;

	while (getline(enemyPopCommands, line))
	{

		// �P�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�p�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		std::string word;
		getline(line_stream, word, ' ');

		// �擪������wEnemy�Ȃ璸�_���W
		if (key == "wEnemy")
		{
			//�G�̐���
			std::unique_ptr<WeakEnemy> newWEnemy = std::make_unique<WeakEnemy>();
			//�G�̏�����
			newWEnemy->WEnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newWEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			//�ړ����� 
			if (word.find("L") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->L);
			}
			else if (word.find("R") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->R);
			}

			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newWEnemy->SetPosition(position);

			newWEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newWEnemy->worldTransform_.UpdateMatrix();
			//�o�^
			wEnemys_.push_back(std::move(newWEnemy));
		}

		// �擪������enemy�Ȃ璸�_���W
		else if (key == "enemy") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy->SetPosition(position);
			newEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy->worldTransform_.UpdateMatrix();
			//�o�^
			enemys_.push_back(std::move(newEnemy));
		}

		else if (key == "wait")
		{
			getline(line_stream, word, ' ');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			isWait_ = true;
			waitTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}

		else if (key == "BOSS")
		{
			isBossScene = true;
			bossCameraPos = viewProjection->GetEye();
		}
	}
}

void GameScene::BossAppears()
{
	if (isBossScene == true)
	{
		bossAppTimer++;
		if (viewProjection->eye.z <= 10)
		{
			viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0, 0.5f));
		}
		if (bossAppTimer >= 180)
		{
			isBossScene = false;
		}
	}
	else if (isBossScene == false && bossAppTimer >= 180)
	{
		if (viewProjection->eye.z > -20)
		{
			viewProjection->SetEye(viewProjection->GetEye() - Vector3(0, 0, 1.0f));
		}
		else
		{
			viewProjection->SetEye(bossCameraPos);
			bossAppTimer = 0;
		}
	}
}
