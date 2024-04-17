/**
* @file GameScene.cpp
* @brief シーンの汎用クラス
* @author イグチ_シュンペイ
*/

#include "DirectXCommon.h"
#include "Input.h"
#include "UI.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "SkyDome.h"
#include "Camera.h"
#include "Sound.h"
#include "Player.h"
#include <memory>

#include <DirectXMath.h>

#pragma once
class GameScene : public GameBaseScene
{
public: //メンバ関数
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Finalize() override;

private:
	SIEngine::Input* input_;
	SIEngine::DirectXCommon* dxCommon_;
	std::unique_ptr<SkyDome> sky_;
	//カメラ
	std::unique_ptr < Camera> camera_;

	//UI
	std::unique_ptr < UI> ui_;
};

