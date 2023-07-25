#include "SIFrameWork.h"

void SIFrameWork::Initialize()
{
	//WindowsAPI�̏�����
	winApp = WinApp::GetInstance();;
	winApp->Initialize();
	//DirectX�̏�����
	dxCommon = DirectXCommon::GetInstance();;
	dxCommon->Initialize(winApp);
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//FBX
	/*FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);*/
	// �p�[�e�B�N���ÓI������
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	// �r���[�v���W�F�N�V�����̏�����
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
}

void SIFrameWork::Finalize()
{
	// WindowsAPI�̏I������
	winApp->Finalize();

	// DirectX���
	dxCommon->fpsFixedFinalize();
}

void SIFrameWork::Update()
{
	// Windows�̃��b�Z�[�W����
	if (winApp->ProcessMessage()) {
		// �Q�[�����[�v�𔲂���
		endRequest = true;
	}

	// ���͂̍X�V
	input->Update();
}

void SIFrameWork::Run()
{
	// �Q�[���̏�����
	Initialize();

	while (true) // �Q�[�����[�v 
	{
		// ���t���[���X�V
		Update();

		// �I�����N�G�X�g�������烋�[�v�𔲂���
		if (IsEndRequest()) {
			// �Q�[�����[�v�𔲂���
			break;
		}

		// �`��
		Draw();
	}

	// �Q�[���̏I��
	Finalize();
}
