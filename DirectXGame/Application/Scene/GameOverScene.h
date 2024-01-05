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
	//カメラ
	ViewProjection* viewProjection_ = nullptr;

	//ロゴやUI
	Sprite* sprite_;
	SpriteCommon spriteCommon_;
	Sprite gameOverLogo_;
	Sprite space_;
	Sprite black_;
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
