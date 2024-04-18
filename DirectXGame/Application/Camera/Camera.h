/**
* @file Camera.h
* @brief カメラ挙動のクラス
* @author イグチ_シュンペイ
*/

#include "ViewProjection.h"
#include "Input.h"
#include "DirectXCommon.h"

#include "UI.h"
#include "Player.h"
#include "BossEnemy.h"

#include <memory>

class Camera
{
public:
	//ゲームシーンの切り替え
	enum BossInitNum
	{
		None,
		Up,     //寄る
		InitCameraWork, //登場カメラワーク
		Loose   //引く
	};
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//プレイヤー登場カメラワーク
	void PlayerInitCameraWork(bool& isStart, Player* player);

	//ボス登場カメラワーク
	void BossInitCameraWork(BossEnemy* bEnemy, bool& isBossInitCamera);

	//clearカメラワーク
	void ToClearCameraWork(bool& isClearCameraWork);

	//gameOverカメラワーク
	void ToGameOverCameraWork(Player* player, bool& isPlayerDead, bool& isGameOver);

	//カメラシェイク
	void CameraShake(int time,float x, float y);

	//カメラ基本移動
	void MoveCamera();

	//Getter
	ViewProjection* GetViewProjection() const { return viewProjection_.get(); }

	//Setter
	static void SetUI(UI* ui) { ui_ = ui; }
	static void SetPlayer(Player* player) { player_ = player; }
	void SetIsShake(bool isShake) { isShake_ = isShake; }

private:
	//カメラ
	std::unique_ptr<ViewProjection> viewProjection_;

	//入力
	SIEngine::Input* input_ = nullptr;
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_ = nullptr;

	//借りてくるもの
	static UI* ui_;
	static Player* player_;

	//target変動地
	float changeTarget_;

	//eyeZ変化値
	float changeEyeZB_;
	float changeEyeZA_;

	//ボス登場番号
	int bossInitNum_;

	//カメラワーク前の座標を入れる変数
	//スタート時
	Vector3 startCameraPos_;
	Vector3 startCamoreMovePos_;
	//死亡時
	Vector3 deadCameraMovePos_;
	//カメラシェイク時
	Vector3 cameraShakePos_;
	//ボス登場時
	Vector3 bossInitCameraPos_;
	//targetを変化させるときに入れる変数
	Vector3 changeTargetNum_;
	Vector3 changeTargetMoveNum_;
	//基本eye
	Vector3 normalEyeNum_;
	//基本target
	Vector3 normalTargetNum_;
	//基本up
	Vector3 normalUpNum_;

	//変化前eye
	Vector3 beforeEyeNum_;
	//変化前target
	Vector3 beforeTargetNum_;
	//変化前up
	Vector3 beforeUpNum_;

	//自機が登場した後の各座標
	Vector3 afterInitCameraPos_;
	Vector3 afterInitCameraTarget_;

	//Vector3すべて０
	Vector3 allZero_;

	//カメラシェイク範囲
	//敵
	float bossCameraShake_;

	//カメラシェイクフラグ
	bool isShake_;

	//ボス登場カメラワーク
	float upZ_;
	float looseZ_;

	//Clearに移行する演出のタイマー
	float clearTimer_;
	//クリア演出中のカメラワークタイマー
	float clearCameraTimer_;
	float clearCameraTimerMax_;
	//クリアカメラ演出番号
	int clearCameraNum_;

	//ボス演出のタイマー
	float bossAppTimer_;
	//最大
	float bossAppTimerMax_;
	//ボス登場タイマー
	int bossInitTimer_;
	int bossInitTimerMax_;
	//スタート演出
	float startTimer_;
	float startTimerMax_;
	//自機死亡タイマー
	float playerDeadTimer_;
	float playerDeadTimerMax_;
	//カメラ移動
	float cameraMoveTimer_;
	float cameraMoveTimerMax_;
	float targetMoveTimer_;
	float targetMoveTimerMax_;

	//移動しているか
	bool isMoveU_;
	bool isMoveD_;
	bool isMoveR_;
	bool isMoveL_;
};