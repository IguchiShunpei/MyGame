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
		NONE,
		Up,     //寄る
		InitCameraWork, //登場カメラワーク
		Loose   //引く
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

	//GameOver画面への演出
	void ToGameOverScene();

	//カメラシェイク
	void CameraShake(float x,float y);

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

	Meteor* objMeteor = nullptr;
	Model* modelMeteor = nullptr;
	Meteor* meteor = nullptr;

	Explosion* explosion01_ = nullptr;
	Explosion* explosion02_ = nullptr;

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
	LevelData* levelData = nullptr;
	//モデル
	std::map<std::string, Model*> meteorModels;
	//オブジェクト
	std::vector<Meteor*> meteorObjects;

	//シーン番号
	int gameNum_;
	//ボス登場番号
	int bossInitNum_;

	//カメラワーク前の座標を入れる変数
	Vector3 cameraWorkPos_;
	Vector3 bossInitCameraPos_;
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

	//フラグ
	//自機が登場したか
	bool isPlayerInit_;
	//ボス戦演出フラグ
	bool isBossInitCamera_;
	//敵を発生させるときの待機フラグ
	bool isWait_;
	//クリア演出フラグ
	bool isClearScene_;
	//敵死亡時のパーティクルフラグ
	bool isBossEffect_;
	//ボス死亡フラグ
	bool isBEnemyDeadScene_;
	//ボスのシェイク
	bool bossShake_;
	//ボスalpha
	bool isBossAlpha_;

	//タイマー
	//打ち出すまでの時間
	float delayTimer_;
	//ボス演出のタイマー
	float bossAppTimer_;
	//敵を発生させるときの待機時間
	int waitTimer_;
	//Clearに移行する演出のタイマー
	float clearTimer_;
	//敵の攻撃に当たった時の演出時間
	int hitTimer_;
	//ボス登場タイマー
	int initTimer_;
};
