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

	viewProjection->Initialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//球の生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//球の初期化
	newEnemy->EnemyInitialize();
	//球の登録
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;

	//カメラ解放
	delete viewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//入力の更新
	input->Update();

	//デスフラグの立った弾を削除
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});

	if (input->PushKey(DIK_UP))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, -0.01f, 0));
	}
	if (input->PushKey(DIK_RIGHT))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(-0.01f, 0, 0));
	}
	if (input->PushKey(DIK_LEFT))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0.01f, 0, 0));
	}
	if (input->PushKey(DIK_DOWN))
	{
		viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0.01f, 0));
	}

	//移動限界座標
	const float kCameraLimitX = 0.28f;
	const float kCameraLimitY = 0.2f;

	//範囲を超えない処理
	viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
	viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
	viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
	viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

	//カメラ
	viewProjection->UpdateMatrix();
	//プレイヤー
	player->Update();
	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
		enemy->ColliderUpdate();
	}

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
	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw(viewProjection);
		enemy->BulletDraw(viewProjection);
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
