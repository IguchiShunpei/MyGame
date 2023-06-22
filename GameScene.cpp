#include "GameScene.h"

void GameScene::Initialize()
{
	SIFrameWork::Initialize();
	
	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//�J����������
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3D�I�u�W�F�N�g����
	fbxObject3d_1 = FbxObject3d::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	fbxObject3d_1->SetModel(fbxModel_1);
	fbxObject3d_1->SetPosition({ 0,-5,20 });
	fbxObject3d_1->SetScale({ 1,1,1 });

	//�p�[�e�B�N��
	particle_1 = Particle::LoadParticleTexture("effect1.png");
	pm_1 = ParticleManager::Create();
	particle_2 = Particle::LoadParticleTexture("effect2.png");
	pm_2 = ParticleManager::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	pm_1->SetParticleModel(particle_1);
	pm_2->SetParticleModel(particle_2);
	//�J�������Z�b�g
	pm_1->SetXMViewProjection(xmViewProjection);
	pm_2->SetXMViewProjection(xmViewProjection);
}

void GameScene::Finalize()
{
	//3D���f�����
	delete fbxModel_1;

	//3D�I�u�W�F�N�g���
	delete fbxObject3d_1;

	//�p�[�e�B�N��
	delete particle_1;
	delete pm_1;
	delete particle_2;
	delete pm_2;

	//FBX���
	FbxLoader::GetInstance()->Finalize();

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

		//�p�[�e�B�N������
		if (input->PushKey(DIK_SPACE))
		{
			pm_1->Fire(particle_1, 30, 0.2f, 0, 2, { 8.0f, 0.0f });
			pm_2->Fire(particle_2, 70, 0.2f, 0, 5, { 4.0f,0.0f });
		}

		//�J����
		viewProjection->UpdateMatrix();
		xmViewProjection->Update();

		//fbx�X�V
		/*fbxObject3d_1->Update();*/

		//�p�[�e�B�N��
		pm_1->Update();
		pm_2->Update();
}

void GameScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

	//3D�I�u�W�F�N�g�`��O����
	FbxObject3d::PreDraw(dxCommon->GetCommandList());

	/*fbxObject3d_1->Draw(viewProjection);*/

	//3D�I�u�W�F�N�g�`��O����
	FbxObject3d::PostDraw();

	//�G�t�F�N�g�`��O����
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//�p�[�e�B�N��
	pm_1->Draw();
	pm_2->Draw();

	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();

	//�����Ƀ|���S���Ȃǂ̕`�揈��������
	/*sprite_1->Draw(dxCommon);
	sprite_2->Draw(dxCommon);*/

#pragma endregion �ŏ��̃V�[���̕`��

	// �`��㏈��
	dxCommon->PostDraw();
}
