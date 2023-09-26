#include "GameClearScene.h"

using namespace DirectX;

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//�J����������
	viewProjection = new ViewProjection();
	viewProjection->Initialize();
}

void GameClearScene::Update()
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

void GameClearScene::Draw()
{
	// �`��O����
	dxCommon->PreDraw();

	Object3d::PreDraw(dxCommon->GetCommandList());

	/*sky->Draw(viewProjection);*/

	Object3d::PostDraw();

	// �`��㏈��
	dxCommon->PostDraw();
}

void GameClearScene::Finalize()
{

}
