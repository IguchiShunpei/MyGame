#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

void GameScene::Initialize()
{
	SIFrameWork::Initialize();

	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//enemy
	enemy = new Enemy;
	enemy->EnemyInitialize();
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;

	//カメラ解放
	delete viewProjection;
	delete xmViewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//入力の更新
	input->Update();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
	{
		// 現在の座標を取得
		Vector3 cameraPos = viewProjection->GetEye();

		// 移動後の座標を計算
		if (input->PushKey(DIK_W)) { cameraPos.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { cameraPos.y -= 0.1f; }
		if (input->PushKey(DIK_A)) { cameraPos.x += 0.1f; }
		else if (input->PushKey(DIK_D)) { cameraPos.x -= 0.1f; }

		// 座標の変更を反映
		viewProjection->SetEye(cameraPos);
	}

	//カメラ
	viewProjection->UpdateMatrix();
	xmViewProjection->Update();

	player->Update();
	enemy->Update();
	enemy->ColliderUpdate();

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

#pragma region 最初のシーンの描画
	 
	Object3d::PreDraw(dxCommon->GetCommandList());

	player->Draw(viewProjection);
	player->BulletDraw(viewProjection);
	if (enemy->GetIsDead() == false)
	{
		enemy->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//エフェクト描画後処理
	ParticleManager::PostDraw();

	//ここにポリゴンなどの描画処理を書く
	/*sprite_1->Draw(dxCommon);
	sprite_2->Draw(dxCommon);*/

#pragma endregion 最初のシーンの描画

	// 描画後処理
	dxCommon->PostDraw();
}
