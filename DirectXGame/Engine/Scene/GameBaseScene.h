/**
* @file GameBaseScene.cpp
* @brief ゲームシーンの基盤クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Sprite.h"

//前方宣言
class GameSceneManager;

//シーン基底クラス
class GameBaseScene
{
public: //メンバ関数
	GameBaseScene() = default;
	virtual ~GameBaseScene() = default;

public: // メンバ関数
	
	//初期化
	virtual void Initialize() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	//解放
	virtual void Finalize() = 0;

public: //メンバ関数
	virtual void SetSceneManager(GameSceneManager* gSceneManager) { gSceneManager_ = gSceneManager; }

private: //メンバ変数
	//シーンマネージャ（借りてくる）
	GameSceneManager* gSceneManager_ = nullptr;
};

