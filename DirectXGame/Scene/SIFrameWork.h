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
	virtual void Draw() = 0;
	//終了チェック
	virtual bool IsEndRequest() { return endRequest; }

public:
	virtual ~SIFrameWork() = default;

public:
	//実行
	void Run();

protected:
	//WindowsAPIのポインタ
	WinApp* winApp = nullptr;
	//DirectXのポインタ
	DirectXCommon* dxCommon = nullptr;
	//入力のポインタ
	Input* input = nullptr;
	// シーンマネージャ
	GameSceneManager* sceneManager_ = nullptr;
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
private:
	bool endRequest = false;
};

