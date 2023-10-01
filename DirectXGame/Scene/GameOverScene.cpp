#include "GameOverScene.h"

using namespace DirectX;

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//�J����������
	viewProjection = new ViewProjection();
	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
}

void GameOverScene::Update()
{
	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) == true)
	{
		//TITLE�i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	// �`��O����
	dxCommon->PreDraw();

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	// �`��㏈��
	dxCommon->PostDraw();
}

void GameOverScene::Finalize()
{
	
}
