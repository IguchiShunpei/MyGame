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
#include"Explosion.h"
#include"Black.h"

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
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	
	//背景
	SkyDome* sky;

	//プレイヤー
	Player* player;

	Explosion* ex;

	//カメラ
	ViewProjection* viewProjection_ = nullptr;

	//ロゴやUI
	Sprite* titleLogo_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* loading = nullptr;
	
	Black* black_ = nullptr;

	//カメラワーク時の黒い縁
	Sprite* outBlack[2];

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
