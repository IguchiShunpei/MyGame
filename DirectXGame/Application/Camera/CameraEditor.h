/**
* @file CameraEditor.h
* @brief 簡単なカメラエディタ
* @author イグチ_シュンペイ
*/

#include "Player.h"
#include "ViewProjection.h"
#include "CameraObject.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "UI.h"
#include "SkyDome.h"
#include "MathFunc.h"

#pragma once
class CameraEditor : public GameBaseScene
{
	//始点か終点か
	enum Point
	{
		Start, //始点
		End  //終点
	};
public:

	~CameraEditor();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Finalize() override;

	//始点と終点追加
	void Add();

	//削除
	void Delete();

	//テスト
	void Test();

	//移動
	void Move(Vector3 start, Vector3 end);

	//ファイル出力
	void OutTextFile();

	//タイトルに戻る
	void ToTitle();

	//カメラ旋回
	void TurnCamera();

private:
	//入力
	SIEngine::Input* input_;
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_;

	//背景
	std::unique_ptr <SkyDome> sky_;
	
	//背景モデル
	std::unique_ptr <Model> skyModel_;

	//プレイヤー
	std::unique_ptr <Player> player_;

	//カメラ
	std::unique_ptr <ViewProjection> viewProjection_;	

	//始点と終点(セット)
	std::list<std::unique_ptr<CameraObject>> cameraObjects_;

	//UI
	std::unique_ptr <UI> ui_;

	//タイトルフラグ
	bool isToTitle_;

	//テストフラグ
	bool isTest_;

	//移動時間
	float moveTime_;
	float movetimeMax_;

	//移動前のカメラ座標
	Vector3 beforeEye_;

	//カメラと対象の距離
	float eyeZ_;

	//y変動値
	float yNum_;

	//アングル
	float angle_;
	float angleNum_;
	//角度
	float r_;
};

