#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <sstream>

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
	viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	LoadEnemy();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect1.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;
	delete p_dmg;
	delete pm_dmg;

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

	//デスフラグの立った敵を削除
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
	const float kCameraLimitX = 0.28f * 1.7f;
	const float kCameraLimitY = 0.2f * 1.7f;

	//範囲を超えない処理
	viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
	viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
	viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
	viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

	//カメラ
	viewProjection->UpdateMatrix();

	//天球
	sky->Update();

	//プレイヤー
	player->Update();
	player->ColliderUpdate();
	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
		enemy->ColliderUpdate();
	}
	

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		if (enemy->GetIsDead() == true)
		{
			pm_dmg->Fire(p_dmg, 50, { enemy->GetWorldPosition().x,enemy->GetWorldPosition().y,enemy->GetWorldPosition().z
				}, 30.0f, 30.0f, 30.0f, 30.0f, 0, 0, 0, 0, 0.2f, 0.2f, 0, 0, 0, 3, { 4.0f, 0.0f });
		}
	}

	pm_dmg->Update();
}

void GameScene::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

#pragma region 最初のシーンの描画

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);
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

	pm_dmg->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

	//ここにポリゴンなどの描画処理を書く
	/*sprite_1->Draw(dxCommon);
	sprite_2->Draw(dxCommon);*/

#pragma endregion 最初のシーンの描画

	// 描画後処理
	dxCommon->PostDraw();
}

void GameScene::LoadEnemy()
{
	enemys_.clear();

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	HRESULT result = S_FALSE;

	std::string num;

	// １行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スパース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		// 先頭文字列がｖなら頂点座標
		if (key == "enemy") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy->EnemyInitialize();
			//コライダーの追加
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy->SetPosition(position);
			//登録
			enemys_.push_back(std::move(newEnemy));
		}
	}
	// ファイルと閉じる
	file.close();
}
