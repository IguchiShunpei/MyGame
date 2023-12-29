/**
* @file GameOverScene.cpp
* @brief ゲームオーバー画面のシーンクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"
#include "SkyDome.h"
#include "Sound.h"
#include "Black.h"

#include <DirectXMath.h>

class GameOverScene : public GameBaseScene
{
public: //メンバ関数
	//コンストラクタ
	GameOverScene();
	//デストラクタ
	~GameOverScene();

public: //メンバ関数
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Finalize() override;

private: // メンバ変数
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	SkyDome* sky_;
	Black* black_;
	//カメラ
	ViewProjection* viewProjection_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* gameOverLogo_ = nullptr;

	bool isToTitle_;
	int toTitleTimer_;

	bool isUp_;
	float logoTime_;
	float logoY_;
};
