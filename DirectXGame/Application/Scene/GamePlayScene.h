/**
* @file GamePlayScene.h
* @brief ゲームのメイン部分
* @author イグチ_シュンペイ
*/

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
#include "InvincibleEnemy.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"
#include "Sound.h"
#include "Explosion.h"
#include "Item.h"
#include "Matrix4.h"
#include "UI.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>
#include <map>
#include <memory>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

struct LevelData;
class CollisionManager;

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

	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }
	const std::list<std::unique_ptr<InvincibleEnemy>>& GetInvEnemys() { return invincibleEnemys_; }

	//アイテムリスト
	const std::list<std::unique_ptr<Item>>& GetItems() { return items_; }

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//リスト削除
	void DeleteObject();

	//プレイヤー登場演出
	void PlayerInit();

	//プレイヤー登場カメラワーク
	void PlayerInitCameraWork();

	//プレイヤー死亡演
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
	void CameraShake(float x, float y);

	//カメラ基本移動
	void MoveCamera();

	//レベルデータのロード
	void LoadLevelData();
private://メンバ変数
	//背景
	std::unique_ptr < SkyDome> sky_;
	//入力
	Input* input_;
	//DxCommon
	DirectXCommon* dxCommon_;

	//カメラ
	std::unique_ptr<ViewProjection> viewProjection_;

	//プレイヤー
	std::unique_ptr <Player> player;

	//敵
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<WeakEnemy> wEnemy;
	std::unique_ptr<InvincibleEnemy> invincibleEnemy;
	std::unique_ptr<BossEnemy> bEnemy;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	std::list<std::unique_ptr<InvincibleEnemy>> invincibleEnemys_;

	//背景
	std::unique_ptr < Meteor> meteor_;
	std::list<std::unique_ptr<Meteor>> meteors_;

	//アイテム
	std::unique_ptr < Item> item_;
	std::list<std::unique_ptr<Item>> items_;

	//UI
	std::unique_ptr < UI> ui_;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;

	//座標
	std::unique_ptr < WorldTransform> worldTransform_;
	//当たり判定
	CollisionManager* collisionManager_;

	//パーティクル
	//弾が当たったエフェクト
	std::unique_ptr < Particle>p_Hit;
	std::unique_ptr < ParticleManager> pm_Hit;
	//雑魚敵が死亡したときのエフェクト
	std::unique_ptr < Particle> p_WDmg;
	std::unique_ptr < ParticleManager> pm_WDmg;
	//ボスが死亡したときのエフェクト
	std::unique_ptr < Particle> p_BDmg;
	std::unique_ptr < ParticleManager> pm_BDmg;
	//隕石を壊した時のエフェクト
	std::unique_ptr < Particle>p_Meteor;
	std::unique_ptr < ParticleManager> pm_Meteor;
	//全般的な爆発エフェクト
	std::unique_ptr < Particle> p_Ex;
	std::unique_ptr < ParticleManager> pm_Ex;
	//自機の死亡時エフェクト
	std::unique_ptr < Particle> p_PEx;
	std::unique_ptr < ParticleManager> pm_PEx;
	//煙
	std::unique_ptr < Particle> p_Smoke;
	std::unique_ptr < ParticleManager> pm_Smoke;

	std::unique_ptr <Explosion> explosion01_;
	std::unique_ptr <Explosion> explosion02_;
	std::unique_ptr <Explosion> explosion03_;

	//レベルデータ
	LevelData* backGroundStar_ = nullptr;
	LevelData* backGroundMeteor_ = nullptr;

	// モデルデータコンテナ
	std::map<std::string, std::unique_ptr<Model>> meteorModels_;
	std::map<std::string, std::unique_ptr<Model>> stardustModels_;
	// オブジェクト
	std::vector<std::unique_ptr<Meteor>> meteorObjects_;
	std::vector<std::unique_ptr<Stardust>> stardustObjects_;

	//シーン番号
	int gameNum_;
	//ボス登場番号
	int bossInitNum_;

	//カメラワーク前の座標を入れる変数
	//スタート時
	Vector3 startCameraPos_;
	Vector3 startCamoreMovePos_;
	//死亡時
	Vector3 deadCameraPos_;
	Vector3 deadCameraMovePos_;
	//カメラシェイク時
	Vector3 cameraShakePos_;
	//ボス登場時
	Vector3 bossInitCameraPos_;
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
	Vector3 changeTargetMoveNum_;

	//自機が登場した後の各座標
	Vector3 afterInitCameraPos_;
	Vector3 afterInitCameraTarget_;

	//Vector3すべて０
	Vector3 allZero_;

	//ボスの死亡座標を保存
	Vector3 bossDeadPos_;
	//自機の死亡座標を保存
	Vector3 playerDeadPos_;
	//自機のクリア演出座標
	Vector3 playerClearPos_;
	float gameClearmoveZ_;
	float gameClearPosZ_;
	//カメラ
	float clearCameraMoveZ_;

	//墜落演出の墜落量
	int gameOverTimer_;
	int gameOverTimerMax_;
	//カメラ移動座標
	int cameraPos_;

	//カメラシェイク範囲
	//自機
	float playerCameraShake_;
	//敵
	float bossCameraShake_;
	float enemyCameraShake_;

	//ボス登場カメラワーク
	float upZ_;
	float looseZ_;
	//黒alpha
	float blackAlphaMax_;

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
	//倒した時のカメラシェイク
	bool isDeadCameraShake_;
	//敵死亡時のパーティクルフラグ
	bool isBossEffect_;
	//ボス死亡フラグ
	bool isBEnemyDeadScene_;
	//スタート演出
	bool isStart_;
	//ゲームオーバー演出
	bool isGameOver_;
	//player死亡フラグ
	bool isPlayerDead_;

	//タイマー
	//打ち出すまでの時間
	float delayTimer_;
	//ボス演出のタイマー
	float bossAppTimer_;
	//最大
	float bossAppTimerMax_;
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
	int hitPlayerTimerMax_;
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
};
