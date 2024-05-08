/**
* @file SlFrameWork.cpp
* @brief メインループを行うクラス
* @author イグチ_シュンペイ
*/

#include "SIFrameWork.h"

void SIFrameWork::Initialize()
{
	//WindowsAPIの初期化
	winApp_ = SIEngine::WinApp::GetInstance();;
	winApp_->Initialize();
	//DirectXの初期化
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();;
	dxCommon_->Initialize(winApp_);
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//入力の初期化
	input_ = SIEngine::Input::GetInstance();
	input_->Initialize(winApp_);
	// ImGuiの初期化
	imGuiManager_ = SIEngine::ImGuiManager::GetInstance();
	imGuiManager_->Initialize(dxCommon_, winApp_);
	// シーンマネージャの生成
	sceneManager_ = GameSceneManager::GetInstance();
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), SIEngine::WinApp::window_width, SIEngine::WinApp::window_height);
	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice());
	// ビュープロジェクションの初期化
	ViewProjection::StaticInitialize(dxCommon_->GetDevice());
	// サウンドの静的初期化
	Sound::StaticInitialize();
}

void SIFrameWork::Finalize()
{
	sceneManager_->Destroy();

	// imguiの終了処理
	imGuiManager_->Finalize();

	// WindowsAPIの終了処理
	winApp_->Finalize();

	// DirectX解放
	dxCommon_->fpsFixedFinalize();
}

void SIFrameWork::Update()
{
	// Windowsのメッセージ処理
	if (winApp_->ProcessMessage()) {
		// ゲームループを抜ける
		endRequest_ = true;
	}

	// 入力の更新
	input_->Update();

	// シーンマネージャの更新
	sceneManager_->Update();
}

void SIFrameWork::Draw()
{
	//シーンマネージャの描画
	sceneManager_->Draw();
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
		if (input_->TriggerKey(DIK_ESCAPE) == true) {
			break;
		}

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();
}
