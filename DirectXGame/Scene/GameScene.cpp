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

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
	viewProjection->SetTarget(player->GetWorldPosition());

	LoadEnemyPop();

	//�p�[�e�B�N��
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

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

	//�����o�ϐ��̏�����
	cameraWorkPos_ = { 0,0,0 };
	//�V�[���ԍ�
	sceneNum = SceneNum::Title;
	//�Q�[�����̃V�[���ԍ�
	gameNum = GameNum::wEnemyScene;
	//�^�C�g���J�������[�N
	titleNum = 0;
	//�{�X
	bossNum_ = 0;
	//�ė���
	gameOverNum_ = 0.0f;

	//�t���O
	isTitleCameraWork_ = false;
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;

	//�^�C�}�[
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
}

void GameScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}

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
	switch (sceneNum)
	{
	case Title:
		//�V��
		sky->Update();
		viewProjection->UpdateMatrix();

		enemys_.clear();
		wEnemys_.clear();

		if (isTitleCameraWork_ == false)
		{
			if (input->TriggerKey(DIK_SPACE))
			{
				isTitleCameraWork_ = true;
			}
		}
		else
		{
			//�^�C�}�[��60�ɂȂ�����titleNum��+����
			titleTimer_++;
			if (titleTimer_ == 60)
			{
				titleNum++;
				titleTimer_ = 0;
				//�J�����̈ʒu��؂�ւ���
				switch (titleNum)
				{
				case 1:
					viewProjection->SetEye(Vector3(0.0f, 0.0f, 10.0f));
					break;
				case 2:
					viewProjection->SetEye(Vector3(0.0f, 2.0f, -20.0f));
					break;
				case 3:
					viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
					viewProjection->SetTarget(Vector3(0.0f, 0.0f, 100.0f));
					break;
				}
			}
			StartCameraWork(titleNum);
			if (titleNum == 4)
			{
				player->SetPosition(Vector3(0, -2, -20));
				viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
				titleNum = 0;
				titleTimer_ = 0;
				isTitleCameraWork_ = false;
				sceneNum = Game;
			}
		}
		break;
	case Game:
		wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
			{
				return wEnemy->GetIsDead();
			});
		//�f�X�t���O�̗������G���폜
		enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
			{
				return enemy->GetIsDead();
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
		case wEnemyScene://�G���G���p�̏���

			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Update();
				wEnemys->ColliderUpdate();
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

				//�����U���ɓ������Ė���������
				if (player->GetIsHit() == false)
				{
					cameraWorkPos_ = viewProjection->GetEye();
				}
				else
				{
					hitTimer_++;
					if (hitTimer_ < 8)
					{
						CameraShake();
					}
					else
					{
						//�ۑ����Ă����ʒu�ɃJ������߂�
						viewProjection->SetEye(cameraWorkPos_);
						player->SetIsHit(false);
						hitTimer_ = 0;
					}
				}

				//�{�X��œG�̎c�萔��0�ɂȂ�����
				if (bossNum_ == 0)
				{
					//�N���A���o�t���O��true
					isClearScene_ = true;
				}

				ToClearScene();
				ToGameOverScene();
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

				//�S�Ă̏Փ˂��`�F�b�N
				collisionManager->CheckAllCollisions();

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

		break;
	case Clear:
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
			Reset();
			viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
			viewProjection->SetTarget(player->GetWorldPosition());
		}
		break;
	case GameOver:
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
			Reset();
			viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
			viewProjection->SetTarget(player->GetWorldPosition());
		}
		break;
	}

}

void GameScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

	Object3d::PreDraw(dxCommon->GetCommandList());

	switch (sceneNum)
	{
	case Title:
		player->Draw(viewProjection);
		sky->Draw(viewProjection);
		break;
	case Game:
		for (auto& object : meteorObjects) {
			object->Draw(viewProjection);
		}
		switch (gameNum)
		{
		case wEnemyScene:
			//�G���G
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Draw(viewProjection);
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
		player->Draw(viewProjection);
		player->BulletDraw(viewProjection);

		break;
	case Clear:
		break;
	case GameOver:
		break;
	case Pose:
		switch (gameNum)
		{
		case wEnemyScene:
			//�G���G
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Draw(viewProjection);
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
		player->Draw(viewProjection);
		player->BulletDraw(viewProjection);

		break;
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

void GameScene::LoadEnemyPop()
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

void GameScene::UpdateEnemyPop()
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

		std::string word;
		getline(line_stream, word, ' ');

		// wEnemy��ǂݎ���č��W���Z�b�g����
		if (key == "wEnemy")
		{
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
		//�ҋ@���Ԃ�ǂݎ��
		else if (key == "wait")
		{
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

void GameScene::BossAppears()
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

void GameScene::ToClearScene()
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
			sceneNum = SceneNum::Clear;
			isClearScene_ = false;
		}
	}
}

void GameScene::ToGameOverScene()
{
	if (player->GetIsDead() == true)
	{
		CameraShake();
		//���@�𓮂���
		player->worldTransform_.position_.y -= 0.05f;
		gameOverNum_ += 0.05f;
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
		player->worldTransform_.UpdateMatrix();
		if (gameOverNum_ >= 3.0f)
		{
			sceneNum = SceneNum::GameOver;
		}
	}
}

void GameScene::CameraShake()
{
	//�����������u
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-0.3f, 0.3f);
	std::uniform_real_distribution<float>dist2(-0.3f, 0.3f);

	viewProjection->eye = viewProjection->eye + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection->UpdateMatrix();
}

void GameScene::Reset()
{
	delete enemy;
	delete wEnemy;

	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));

	LoadEnemyPop();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//�����o�ϐ��̏�����
	cameraWorkPos_ = { 0,0,0 };
	//�V�[���ԍ�
	sceneNum = SceneNum::Title;
	//�Q�[�����̃V�[���ԍ�
	gameNum = GameNum::wEnemyScene;
	//�^�C�g���J�������[�N
	titleNum = 0;
	//�{�X
	bossNum_ = 0;
	//�ė���
	gameOverNum_ = 0.0f;

	//�t���O
	isTitleCameraWork_ = false;
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;


	//�^�C�}�[
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;

}

void GameScene::StartCameraWork(int num)
{
	switch (num)
	{
	case 0:
		viewProjection->eye.x = 15.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		viewProjection->eye.y = 10.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 1:
		viewProjection->eye.x = 20.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 2:
		viewProjection->eye.z = 30.0f * MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 3:
		//���@�𓮂���
		player->worldTransform_.position_.z++;
		viewProjection->eye.z -= 1.5f;
		player->worldTransform_.rotation_.z = 360.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
		player->worldTransform_.UpdateMatrix();
		break;
	}
}
