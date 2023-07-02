#pragma once
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "ParticleManager.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class CollisionManager;
class Player;

class GameScene : public SIFrameWork
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;

private:
	//入力
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//カメラ
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//プレイヤー
	Player* player = nullptr;
	//敵
	Enemy* enemy = nullptr;

	//座標
	WorldTransform* worldTransform = nullptr;
	//当たり判定
	CollisionManager* collisionManager = nullptr;
};
