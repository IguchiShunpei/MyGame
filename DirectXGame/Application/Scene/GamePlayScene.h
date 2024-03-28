/**
* @file GamePlayScene.h
* @brief ゲームのメイン部分
* @author イグチ_シュンペイ
*/

#pragma once

#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Player.h"
#include "ShotEnemy.h"
#include "InvincibleEnemy.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"
#include "Sound.h"
#include "Item.h"
#include "UI.h"
#include "Camera.h"
#include "MathFunc.h"
#include "Effect.h"
#include "BackGround.h"
#include "SkyDome.h"

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
	const std::list<std::unique_ptr<ShotEnemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }
	const std::list<std::unique_ptr<InvincibleEnemy>>& GetInvEnemys() { return invincibleEnemys_; }

	//アイテムリスト
	const std::list<std::unique_ptr<Item>>& GetItems() { return items_; }

	//エフェクト処理
	void EffectUpdate(bool& isDeadCameraShake_);

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//リスト削除
	void DeleteObject();

	//プレイヤー登場演出
	void PlayerInit();

	//プレイヤー死亡演
	void PlayerDead();

	//BOSS戦前の演出
	void BossInit();

	//ボス敵の死亡演出
	void BossDead();

	//Clear画面への演出
	void ToClearScene();

	//GameOver画面への演出
	void ToGameOverScene();

private://メンバ変数
	//入力
	SIEngine::Input* input_;
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_;

	//背景
	std::unique_ptr < SkyDome> sky_;

	//プレイヤー
	std::unique_ptr <Player> player_;

	//敵
	std::unique_ptr<ShotEnemy> enemy;
	std::unique_ptr<WeakEnemy> wEnemy;
	std::unique_ptr<InvincibleEnemy> invincibleEnemy;
	std::unique_ptr<BossEnemy> bEnemy;

	//敵
	std::list<std::unique_ptr<ShotEnemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	std::list<std::unique_ptr<InvincibleEnemy>> invincibleEnemys_;

	//アイテムを持っている敵
	std::unique_ptr < Meteor> meteor_;
	std::list<std::unique_ptr<Meteor>> meteors_;

	//背景
	std::unique_ptr < BackGround> backGround_;

	//アイテム
	std::unique_ptr < Item> item_;
	std::list<std::unique_ptr<Item>> items_;

	//UI
	std::unique_ptr < UI> ui_;

	//カメラ
	std::unique_ptr < Camera> camera_;

	//エフェクト
	std::unique_ptr < Effect> effect_;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;

	//座標
	std::unique_ptr < WorldTransform> worldTransform_;
	//当たり判定
	CollisionManager* collisionManager_;

	//シーン番号
	int gameNum_;
	//ボス登場番号
	int bossInitNum_;

	//カメラシェイク範囲
	//自機
	float playerCameraShake_;
	//敵
	float bossCameraShake_;
	float enemyCameraShake_;

	//ボスの死亡座標を保存
	Vector3 bossDeadPos_;
	//自機の死亡座標を保存
	Vector3 playerDeadPos_;
	Vector3 deadCameraPos_;
	//自機のクリア演出座標
	Vector3 playerClearPos_;
	float gameClearmoveZ_;
	float gameClearPosZ_;
	//カメラ
	float clearCameraMoveZ_;

	//墜落演出の墜落量
	int gameOverTimer_;
	int gameOverTimerMax_;

	//基本eye
	Vector3 normalEyeNum_;
	//基本target
	Vector3 normalTargetNum_;
	//基本up
	Vector3 normalUpNum_;

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
	//敵を発生させるときの待機時間
	int waitTimer_;

	//敵が死んだ時の演出時間
	int hitEnemyTimer_;

	//自機が攻撃に当たった時の演出時間
	int hitPlayerTimer_;
	int hitPlayerTimerMax_;

};
