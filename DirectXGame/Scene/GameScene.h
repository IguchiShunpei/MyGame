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

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class CollisionManager;
class Player;

class GameScene : public SIFrameWork
{
public:
	enum SceneNum
	{
		Title,
		Game,
		Clear,
		GameOver,
		Pose,
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

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

	//BOSS戦前の演出
	void BossAppears();

private:
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

	//シーン番号
	int sceneNum;
	//wave番号
	int waveNum;
	//ボス戦に入ったか
	bool isBossScene;
	//ボス演出のタイマー
	int bossAppTimer;
	//ボス戦のカメラワーク前の座標を入れる変数
	Vector3 bossCameraPos;

	//打ち出すまでの時間
	float delayTimer = 0.0f;

	bool isWait_ = false;

	int waitTimer = 10;
};
