/**
* @file GameClearScene.cpp
* @brief ゲームクリア画面のシーンクラス
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
#include <memory>

#include <DirectXMath.h>

class GameClearScene : public GameBaseScene
{
public: //メンバ関数
	//コンストラクタ
	GameClearScene();
	//デストラクタ
	~GameClearScene();

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
	SIEngine::Input* input_ = nullptr;
	SIEngine::DirectXCommon* dxCommon_ = nullptr;
	std::unique_ptr<SkyDome> sky_;
	//カメラ
	std::unique_ptr < ViewProjection> viewProjection_;

	//ロゴやUI
	std::unique_ptr < SIEngine::Sprite> sprite_;
	SIEngine::SpriteCommon spriteCommon_;
	SIEngine::Sprite clearLogo_;
	SIEngine::Sprite space_;
	SIEngine::Sprite black_;
	//黒alpha値
	float blackAlpha_;
	//変動値
	float blackAlphaNum_;
	//最大値
	float blackAlphaNumMax_;
	//最小値
	float blackAlphaNumMin_;

	bool isToTitle_;
	int toTitleTimer_;

	bool isUp_;
	float logoTime_;
	float logoY_;
};
