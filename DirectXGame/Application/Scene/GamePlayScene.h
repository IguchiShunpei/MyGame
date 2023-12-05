#pragma once

#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameClearScene.h"
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "SkyDome.h"
#include "LevelLoader.h"
#include "Stardust.h"
#include "Meteor.h"
#include "Player.h"
#include "Enemy.h"
#include "InvEnemy.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"
#include "Sound.h"
#include "Explosion.h"
#include "Black.h"
#include "Red.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>
#include <map>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

struct LevelData;

class CollisionManager;
class Player;

//ゲームプレイシーン
class GamePlayScene : public GameBaseScene
{
public:
	//ゲームシーンの切り替え
	enum GameNum
	{
		FirstScene, //道中
		BossScene  //ボス戦
	};
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
	//カメラ位置切り替え
	enum CameraPos
	{
		Front,
		Back
	};

public:
	GamePlayScene();
	~GamePlayScene();

public:
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Finalize() override;

	//ゲームのリセット
	void GameReset();

	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }
	const std::list<std::unique_ptr<InvEnemy>>& GetInvEnemys() { return invEnemys_; }

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//プレイヤー登場演出
	void PlayerInit();

	//プレイヤー登場カメラワーク
	void PlayerInitCameraWork();

	//プレイヤー死亡演出
	void PlayerDead();

	//BOSS戦前の演出
	void BossInit();

	//プレイヤー登場カメラワーク
	void BossInitCameraWork();

	//ボス敵の死亡演出
	void BossDead();

	//Clear画面への演出
	void ToClearScene();

	//clearカメラワーク
	void ToClearCameraWork();

	//GameOver画面への演出
	void ToGameOverScene();

	//gameOverカメラワーク
	void ToGameOverCameraWork();

	//カメラシェイク
	void CameraShake(float x,float y);

	//カメラ向き変更
	void ChangeCameraDirection();
	void CameraMovePoint();

	//カメラ基本移動
	void MoveCamera();

	//レベルデータのロード
	void LoadLevelData();

	//UIの処理が多いので関数化
	void UIInitialize();
	void UIInitMotion();
	void UIOutMotion();
	void UIUpdate();
	void UIDraw();
	void UIMove();

private://メンバ変数
	//背景
	SkyDome* sky_;
	//入力
	Input* input_ = nullptr;
	//DxCommon
	DirectXCommon* dxCommon_ = nullptr;

	//カメラ
	ViewProjection* viewProjection_ = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//プレイヤー
	Player* player;

	//敵
	Enemy* enemy;
	WeakEnemy* wEnemy;
	InvEnemy* invEnemy;
	BossEnemy* bEnemy;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	std::list<std::unique_ptr<InvEnemy>> invEnemys_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//座標
	WorldTransform* worldTransform = nullptr;
	//当たり判定
	CollisionManager* collisionManager = nullptr;

	//パーティクル
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;

	Particle* p_wDmg = nullptr;
	ParticleManager* pm_eDmg = nullptr;

	Particle* p_bDmg = nullptr;
	ParticleManager* pm_bDmg = nullptr;

	Stardust* objStardust = nullptr;
	Model* modelStardust = nullptr;
	Stardust* stardust = nullptr;

	Meteor* objMeteor = nullptr;
	Model* modelMeteor = nullptr;
	Meteor* meteor = nullptr;

	Explosion* explosion01_ = nullptr;
	Explosion* explosion02_ = nullptr;
	Explosion* explosion03_ = nullptr;

	Black* black_ = nullptr;
	Red* red_ = nullptr;

	//UI
	Sprite* arrowUpOn = nullptr;
	Sprite* arrowUpOff = nullptr;
	Sprite* arrowDownOn = nullptr;
	Sprite* arrowDownOff = nullptr;
	Sprite* arrowRightOn = nullptr;
	Sprite* arrowRightOff = nullptr;
	Sprite* arrowLeftOn = nullptr;
	Sprite* arrowLeftOff = nullptr;

	//各方向フラグ
	bool isUp_;
	bool isDown_;
	bool isRight_;
	bool isLeft_;
	bool isNeutral_;
	bool isUIInit_;
	bool isUIOut_;

	//登場モーション範囲
	float UIInitRange_;
	//登場モーション時間
	float UIInitTime_;
	//登場モーション移動量
	float UIInitPos_;
	//退場モーション範囲
	float UIOutRange_;
	//退場モーション時間
	float UIOutTime_;
	//退場モーション移動量
	float UIOutPos_;

	//登場後モーション
	//フラグ
	bool isMove_;
	float UIMoveTime_;
	float UIMovePos_;
	float UIMoveRange_;

	//レベルデータ
	//星屑
	LevelData* backGroundStardust_ = nullptr;
	//モデル
	std::map<std::string, Model*> stardustModels_;
	//オブジェクト
	std::vector<Stardust*> stardustObjects_;

	//隕石
	LevelData* backGroundMeteor_ = nullptr;
	//モデル
	std::map<std::string, Model*> meteorModels_;
	//オブジェクト
	std::vector<Meteor*> meteorObjects_;


	//シーン番号
	int gameNum_;
	//ボス登場番号
	int bossInitNum_;

	//カメラワーク前の座標を入れる変数
	//スタート時
	Vector3 startCameraPos_;
	//死亡時
	Vector3 deadCameraPos_;
	//カメラシェイク時
	Vector3 cameraShakePos_;
	//ボス登場時
	Vector3 bossInitCameraPos_;
	//カメラの向きが変わる時
	//移動先
	Vector3 cameraDirChangePos_;
	//中間点
	Vector3 cameraMovePoint_;
	//中間点までのz移動量
	float cameraMoveZ_;
	//targetZ移動量
	float moveTargetZ_;
	//移動前の座標を保存
	Vector3 beforeMoveCameraPos_;
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
	//targetを変化させるときに入れる変数
	Vector3 changeTargetNum_;
	//ボスの死亡座標を保存
	Vector3 bossDeadPos_;
	//自機の死亡座標を保存
	Vector3 playerDeadPos_;
	//自機のクリア演出座標
	Vector3 playerClearPos_;
	
	//墜落演出の墜落量
	int gameOverNum_;
	//ボスの墜落時のシェイク範囲
	float shakeRange_;
	float shakeNum_;
	//ボスの墜落スピード
	float bossDownSpeed_;
	//ボスのalpha 
	float bossAlpha_;
	//ボスalphaに代入する変数
	float bossAlphaNum_;
	//カメラ移動座標
	int cameraPos_;

	//フラグ
	//自機が登場したか
	bool isPlayerInit_;
	//ボス戦演出フラグ
	bool isBossInitCamera_;
	//敵を発生させるときの待機フラグ
	bool isWait_;
	//クリア演出フラグ
	bool isClearScene_;
	//クリア画面カメラワーク
	bool isClearCameraWork_;
	//敵を倒した時のカメラシェイク
	bool isEnemyDeadCameraShake_;
	//敵死亡時のパーティクルフラグ
	bool isBossEffect_;
	//ボス死亡フラグ
	bool isBEnemyDeadScene_;
	//ボスのシェイク
	bool bossShake_;
	//ボスalpha
	bool isBossAlpha_;
	//スタート演出
	bool isStart_;
	//ゲームオーバー演出
	bool isGameOver_;
	//カメラ向き変更フラグ
	bool isChangeCameraDir_;
	//カメラ移動時に中間点を通ったか
	bool isPassPoint_;
	//player死亡フラグ
	bool isPlayerDead_;

	//タイマー
	//打ち出すまでの時間
	float delayTimer_;
	//ボス演出のタイマー
	float bossAppTimer_;
	//敵を発生させるときの待機時間
	int waitTimer_;
	//Clearに移行する演出のタイマー
	float clearTimer_;
	int playerShakeTimer_;
	float clearCameraTimer_;
	float clearCameraTimerMax_;
	int clearCameraNum_;
	//敵が死んだ時の演出時間
	int hitEnemyTimer_;
	//自機が攻撃に当たった時の演出時間
	int hitPlayerTimer_;
	//ボス登場タイマー
	int bossInitTimer_;
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
};
