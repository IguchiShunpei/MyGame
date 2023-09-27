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

private: // メンバ変数
	// 入力
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	
	//背景
	SkyDome* sky;

	//プレイヤー
	Player* player;

	//カメラ
	ViewProjection* viewProjection = nullptr;

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
};
