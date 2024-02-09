/**
 * @file GameTitleScene.cpp
 * @brief タイトル画面のシーンクラス
 * @author イグチ_シュンペイ
 */

#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"
#include "SkyDome.h"
#include "viewProjection.h"
#include "Sound.h"
#include "Player.h"
#include <memory>

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: //メンバ関数
	//コンストラクタ
	GameTitleScene();

	//デストラクタ
	~GameTitleScene();

public: //メンバ関数
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Finalize() override;

	//タイトルシーンのカメラワーク
	void StartCameraWork(int num);

	//ロゴ移動
	void LogoOut();

private: // メンバ変数
	// 入力
	Input* input_;
	DirectXCommon* dxCommon_;
	
	//背景
	std::unique_ptr <SkyDome> sky;

	//プレイヤー
	std::unique_ptr <Player> player;

	//カメラ
	std::unique_ptr <ViewProjection> viewProjection_;

	//ロゴやUI
	std::unique_ptr <Sprite> sprite_;
	SpriteCommon spriteCommon_;
	Sprite titleLogo_;
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

	//タイトル
	float logoTime_;
	float logoY_;

	//自機の角度を保存
	float plRotaY;
	//天球の角度を保存
	float skyRotaY;

	//タイトルカメラワーク
	int titleNum;
	//カメラワーク前の演出のフラグ
	bool isBeforeCameraWork_;
	//タイトルカメラワークのフラグ
	bool isTitleCameraWork_;
	//タイトルシーンのカメラワーク
	int titleTimer_;

	//カメラ座標を保存する変数
	Vector3 cameraWorkPos_;
};