#include "SIFrameWork.h"

void SIFrameWork::Initialize()
{
	//WindowsAPIの初期化
	winApp = WinApp::GetInstance();;
	winApp->Initialize();
	//DirectXの初期化
	dxCommon = DirectXCommon::GetInstance();;
	dxCommon->Initialize(winApp);
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);
	// シーンマネージャの生成
	sceneManager_ = GameSceneManager::GetInstance();
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//FBX
	/*FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);*/
	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	// ビュープロジェクションの初期化
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
	// サウンドの静的初期化
	Sound::StaticInitialize();
}

void SIFrameWork::Finalize()
{
	sceneManager_->Destroy();
	//シーンファクトリの解放
	delete sceneFactory_;

	// WindowsAPIの終了処理
	winApp->Finalize();

	// DirectX解放
	dxCommon->fpsFixedFinalize();
}

void SIFrameWork::Update()
{
	// Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		// ゲームループを抜ける
		endRequest = true;
	}

	// 入力の更新
	input->Update();

	// シーンマネージャの更新
	sceneManager_->Update();
}

void SIFrameWork::Run()
{
	// ゲームの初期化
	Initialize();

	while (true) // ゲームループ 
	{
		// 毎フレーム更新
		Update();

		// 終了リクエストが来たらループを抜ける
		if (IsEndRequest()) {
			// ゲームループを抜ける
			break;
		}
		//escでループを抜ける
		if (input->TriggerKey(DIK_ESCAPE) == true) {
			break;
		}

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();
}
