#pragma once
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "WeakEnemy.h"
#include "ParticleManager.h"
#include "SkyDome.h"
#include "LevelLoader.h"
#include "Meteor.h"

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

class GameScene : public SIFrameWork
{
public:
	//画面切り替え
	enum SceneNum
	{
		Title,
		Game,
		Clear,
		GameOver,
		Pose,
	};

	//ゲームシーンの切り替え
	enum GameNum
	{
		wEnemyScene,
		BossScene,
	};

public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;

	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }

	const int GetBossNum() { return bossNum; }

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//BOSS戦前の演出
	void BossAppears();

	//Clear画面への演出
	void ToClearScene();

	//GameOver画面への演出
	void ToGameOverScene();

	//カメラシェイク
	void CameraShake();

	//リセット
	void Reset();

	//タイトルシーンのカメラワーク
	void StartCameraWork(int num);

private://メンバ変数
	//背景
	SkyDome* sky;
	//入力
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//カメラ
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//プレイヤー
	Player* player;

	//敵
	Enemy* enemy;
	WeakEnemy* wEnemy;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	
	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//座標
	WorldTransform* worldTransform = nullptr;
	//当たり判定
	CollisionManager* collisionManager = nullptr;

	//パーティクル
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;

	Meteor* objMeteor = nullptr;

	Model* modelMeteor = nullptr;

	Meteor* meteor;

	//レベルデータ
	LevelData* levelData = nullptr;
	//モデル
	std::map<std::string, Model*> meteorModels;
	//オブジェクト
	std::vector<Meteor*> meteorObjects;

	//シーン番号
	int sceneNum;
	int gameNum;
	//wave番号
	int waveNum;
	//タイトルカメラワーク
	int titleNum;
	//カメラワーク前の座標を入れる変数
	Vector3 cameraWorkPos_;
	//ボスの数
	int bossNum;

	//フラグ
	//タイトルカメラワークのフラグ
	bool isTitleCameraWork_;
	//ボス戦に入ったか
	bool isBossScene_;
	//敵を発生させるときの待機フラグ
	bool isWait_;
	//クリア演出フラグ
	bool isClearScene_;

	//タイマー
	//タイトルシーンのカメラワーク
	int titleTimer_;
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
};
