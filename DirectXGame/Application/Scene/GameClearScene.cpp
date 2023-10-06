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

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//カメラ初期化
	viewProjection = new ViewProjection();
	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
}

void GameClearScene::Update()
{
	//天球
	sky->Update();
	viewProjection->UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) == true)
	{
		//TITLE（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	// 描画後処理
	dxCommon->PostDraw();
}

void GameClearScene::Finalize()
{

}
