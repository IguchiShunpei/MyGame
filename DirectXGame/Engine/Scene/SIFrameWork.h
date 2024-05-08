/**
* @file SlFrameWork.cpp
* @brief メインループを行うクラス
* @author イグチ_シュンペイ
*/

#pragma once
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include"Model.h"
#include "ParticleManager.h"
#include "FbxLoader.h"
#include "AbstractSceneFactory.h"
#include "GameSceneManager.h"
#include "ImGuiManager.h"
#include  "Sound.h"

class SIFrameWork
{
public://メンバ関数
	//初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw();
	//終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

public:
	virtual ~SIFrameWork() = default;

public:
	//実行
	void Run();

protected:
	//WindowsAPIのポインタ
	SIEngine::WinApp* winApp_ = nullptr;
	//DirectXのポインタ
	SIEngine::DirectXCommon* dxCommon_ = nullptr;
	//入力のポインタ
	SIEngine::Input* input_ = nullptr;
	// シーンマネージャ
	GameSceneManager* sceneManager_ = nullptr;
	// シーンファクトリー
	std::unique_ptr <AbstractSceneFactory> sceneFactory_;
	//imgui
	SIEngine::ImGuiManager* imGuiManager_ = nullptr;
private:
	bool endRequest_ = false;
};

