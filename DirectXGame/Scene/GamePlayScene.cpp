#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <string.h>
#include <math.h>


using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//�����蔻��
	collisionManager = CollisionManager::GetInstance();

	//�J����������
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();
	viewProjection->Initialize();

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	LoadEnemyPop();

	//�p�[�e�B�N��
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	LoadLevelData();

	//�����o�ϐ��̏�����
	cameraWorkPos_ = { 0,0,0 };
	//�Q�[�����̃V�[���ԍ�
	gameNum = GameNum::FirstScene;
	//�{�X
	bossNum_ = 0;
	//�ė���
	gameOverNum_ = 0;

	//�t���O
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;

	//�^�C�}�[
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
}

void GamePlayScene::Update()
{
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDead();
		});
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDelete();
		});
	//�f�X�t���O�̗������G���폜
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	invEnemys_.remove_if([](std::unique_ptr <InvEnemy>& invEnemy)
		{
			return invEnemy->GetIsDelete();
		});

	//�X�V�R�}���h
	UpdateEnemyPop();

	//�V��
	sky->Update();
	//���@�̓o�ꉉ�o
	player->IntitMotion();
	//�{�X�̓o�ꉉ�o
	BossAppears();

	//�J����
	viewProjection->UpdateMatrix();

	for (auto& object : meteorObjects)
	{
		object->MeteorUpdate();
	}

	switch (gameNum)
	{
	case FirstScene://�G���G���p�̏���

		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->ColliderUpdate();
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
		{
			invEnemys->Update();
			invEnemys->ColliderUpdate();
		}
		break;

	case BossScene://�{�X���p�̏���

		//�{�X�o�ꉉ�o�t���O��false�ɂȂ�����{�X��J�n
		if (isBossScene_ == false)
		{
			//�G
			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				enemys->Update();
				enemys->ColliderUpdate();
			}

			//�{�X��œG�̎c�萔��0�ɂȂ�����
			if (bossNum_ == 0)
			{
				//�N���A���o�t���O��true
				isClearScene_ = true;
			}
			//�N���A���o�̏���
			ToClearScene();
		}

		break;
	}

	if (player->GetIsInit() == true)
	{
		if (isBossScene_ == false && isClearScene_ == false)
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

			player->Update();
			player->ColliderUpdate();

			//�p�[�e�B�N���X�V
			pm_dmg->Update();
		}
		else
		{
			//�p�[�e�B�N���X�V
			pm_dmg->Update();
			player->BulletUpdate();
		}
	}

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	//�����U���ɓ������Ė���������
	if (player->GetIsDead() == false)
	{
		if (player->GetIsHit() == false)
		{
			cameraWorkPos_ = viewProjection->GetEye();
		}
		else
		{
			if (player->GetIsInv() == true)
			{
				hitTimer_++;
				if (hitTimer_ < 16)
				{
					CameraShake();
				}
				else
				{
					//�ۑ����Ă����ʒu�ɃJ������߂�
					viewProjection->SetEye(cameraWorkPos_);
					player->SetIsHit(false);
					player->SetIsInv(false);
					hitTimer_ = 0;
				}
			}
		}
	}

	//�G���S���̃p�[�e�B�N��
	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		if (wEnemys->GetIsDead() == true)
		{
			Vector3 deadPos{};
			deadPos = wEnemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ deadPos.x,deadPos.y,deadPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
		}
	}

	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		if (enemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = enemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ hitPos.x,hitPos.y,hitPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 4, { 2.0f, 0.0f });
		}
		if (enemys->GetIsDead() == true)
		{
			Vector3 deadPos{};
			deadPos = enemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ deadPos.x,deadPos.y,deadPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
			bossNum_--;
		}
	}
	ToGameOverScene();
}

void GamePlayScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

	Object3d::PreDraw(dxCommon->GetCommandList());

	switch (gameNum)
	{
	case FirstScene:
		//�G���G
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Draw(viewProjection);
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
		{
			invEnemys->Draw(viewProjection);
		}
		break;

	case BossScene:
		//�{�X�G
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			if (enemys->GetIsHit() == false)
			{
				enemys->Draw(viewProjection);
			}
			enemys->BulletDraw(viewProjection);
		}
		break;
	}

	sky->Draw(viewProjection);
	//�J�����V�F�C�N���͓_��
	if (hitTimer_ % 2 != 1)
	{
		player->Draw(viewProjection);
	}
	player->BulletDraw(viewProjection);

	for (auto& object : meteorObjects) {
		object->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//�G�t�F�N�g�`��O����
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	pm_dmg->Draw();

	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();

#pragma endregion �ŏ��̃V�[���̕`��

	// �`��㏈��
	dxCommon->PostDraw();
}

void GamePlayScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}

	delete sky;
	delete player;
	delete enemy;
	delete wEnemy;
	delete invEnemy;
	delete p_dmg;
	delete pm_dmg;

	//�J�������
	delete viewProjection;
	delete xmViewProjection;
}

void GamePlayScene::GameReset()
{
	viewProjection->Initialize();
}

void GamePlayScene::LoadEnemyPop()
{
	enemys_.clear();
	wEnemys_.clear();

	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	// �t�@�C���ƕ���
	file.close();
}

void GamePlayScene::UpdateEnemyPop()
{
	//�ҋ@����
	if (isWait_ == true)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
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

		// wEnemy��ǂݎ���č��W���Z�b�g����
		if (key == "wEnemy")
		{
			std::string word;
			getline(line_stream, word, ' ');
			//�G�̐���
			std::unique_ptr<WeakEnemy> newWEnemy = std::make_unique<WeakEnemy>();
			//�G�̏�����
			newWEnemy->WEnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newWEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			//�ړ�������ǂݎ���ăZ�b�g����
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

		// enemy��ǂݎ���č��W���Z�b�g����
		else if (key == "enemy") {
			std::string word;
			getline(line_stream, word, ' ');
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			if (word.find("HP") == 0)
			{
				std::string num;
				getline(line_stream, num, ' ');
				//hp��ۑ�
				int32_t hpNum = atoi(num.c_str());
				newEnemy->SetHp(hpNum);
			}
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
			bossNum_++;
		}
		// enemy��ǂݎ���č��W���Z�b�g����
		else if (key == "invEnemy") {
			//�G�̐���
			std::unique_ptr<InvEnemy> newInvEnemy = std::make_unique<InvEnemy>();
			//�G�̏�����
			newInvEnemy->InvEnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newInvEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newInvEnemy->SetPosition(position);
			newInvEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newInvEnemy->worldTransform_.UpdateMatrix();
			//�o�^
			invEnemys_.push_back(std::move(newInvEnemy));
		}
		//�ҋ@���Ԃ�ǂݎ��
		else if (key == "wait")
		{
			std::string word;
			getline(line_stream, word, ' ');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			isWait_ = true;
			waitTimer_ = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}
		//"BOSS"��ǂݎ���ă{�X��Ɉڍs
		else if (key == "BOSS")
		{
			isBossScene_ = true;
			gameNum = GameNum::BossScene;
			cameraWorkPos_ = viewProjection->GetEye();
		}
	}
}

void GamePlayScene::BossAppears()
{
	//�ڋ߂���J�������[�N
	if (isBossScene_ == true)
	{
		if (viewProjection->eye.z <= 10)
		{
			viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0, 10) * MathFunc::easeInSine(bossAppTimer_ / 180.0f));
		}
		if (bossAppTimer_ >= 180)
		{
			isBossScene_ = false;
		}
		bossAppTimer_++;
	}
	//�����J�������[�N
	else if (isBossScene_ == false && bossAppTimer_ >= 180)
	{
		if (viewProjection->eye.z > -20)
		{
			viewProjection->SetEye(viewProjection->GetEye() - Vector3(0, 0, 1.0f));
		}
		else
		{
			viewProjection->SetEye(cameraWorkPos_);
			bossAppTimer_ = 0;
		}
	}
}
void GamePlayScene::ToClearScene()
{
	if (isClearScene_ == true)
	{
		//���@�𓮂���
		player->worldTransform_.position_.z++;
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
		player->worldTransform_.UpdateMatrix();
		//���@���ړ�����������V�[���J��
		if (player->worldTransform_.position_.z >= 100)
		{

			GameSceneManager::GetInstance()->ChangeScene("CLEAR");
			isClearScene_ = false;
		}
	}
}
void GamePlayScene::ToGameOverScene()
{
	if (player->GetIsDead() == true)
	{
		CameraShake();
		//���@�𓮂���
		player->worldTransform_.position_.y -= 0.05f;
		gameOverNum_++;
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
		player->worldTransform_.UpdateMatrix();
		if (gameOverNum_ >= 60)
		{
			GameSceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}
}
void GamePlayScene::CameraShake()
{
	//�����������u
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-0.3f, 0.3f);
	std::uniform_real_distribution<float>dist2(-0.3f, 0.3f);

	viewProjection->eye = viewProjection->eye + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection->UpdateMatrix();
}

void GamePlayScene::LoadLevelData()
{
	// ���x���f�[�^�̓ǂݍ���
	levelData = LevelLoader::LoadFile("backGround");

	//���f���ǂݍ���
	modelMeteor = Model::LoadFromOBJ("meteor");

	meteorModels.insert(std::make_pair("meteor", modelMeteor));

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(meteorModels)::iterator it = meteorModels.find(objectData.fileName);
		if (it != meteorModels.end()) {
			model = it->second;
		}

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		meteor = new Meteor;
		meteor->MeteorInitialize();
		meteor->SetModel(model);

		// ���W
		Vector3 pos;
		//�f�[�^�̒l����
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetPosition(pos);

		// ��]�p
		Vector3 rot;
		//�f�[�^�̒l����
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetRotation(rot);

		// ���W
		Vector3 scale;
		//�f�[�^�̒l����
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetScale(scale);

		// �z��ɓo�^
		meteorObjects.push_back(meteor);
	}
}
