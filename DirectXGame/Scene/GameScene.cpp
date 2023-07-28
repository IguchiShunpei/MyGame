#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>

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

	LoadEnemyPop();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect1.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	//メンバ変数の初期化
	bossCameraPos = { 0,0,0 };

	//フラグ
	isBossScene = false;
	sceneNum = 0;
	bossAppTimer = 0;
}

void GameScene::Finalize()
{
	delete player;
	delete enemy;
	delete wEnemy;
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

	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDead();
		});

	//天球
	sky->Update();

	player->IntitMotion();

	BossAppears();

	//カメラ
	viewProjection->UpdateMatrix();

	if (player->GetIsInit() == true)
	{
		//更新コマンド
		UpdateEnemyPop();

		if (isBossScene == false)
		{
			if (input->PushKey(DIK_UP))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, -0.05f, 0));
			}
			if (input->PushKey(DIK_RIGHT))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(-0.05f, 0, 0));
			}
			if (input->PushKey(DIK_LEFT))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0.05f, 0, 0));
			}
			if (input->PushKey(DIK_DOWN))
			{
				viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0.05f, 0));
			}

			//移動限界座標
			const float kCameraLimitX = 1.5f * 1.7f;
			const float kCameraLimitY = 1.1f * 1.7f;

			//範囲を超えない処理
			viewProjection->eye.x = max(viewProjection->eye.x, -kCameraLimitX);
			viewProjection->eye.x = min(viewProjection->eye.x, +kCameraLimitX);
			viewProjection->eye.y = max(viewProjection->eye.y, -kCameraLimitY);
			viewProjection->eye.y = min(viewProjection->eye.y, +kCameraLimitY);

			//プレイヤー
			player->Update();
			player->ColliderUpdate();

			//敵
			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				enemys->Update();
				enemys->ColliderUpdate();
			}
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Update();
				wEnemys->ColliderUpdate();
			}

			//全ての衝突をチェック
			collisionManager->CheckAllCollisions();

			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				if (enemys->GetIsDead() == true)
				{
					Vector3 deadPos{};
					deadPos = enemys->GetPosition();
					pm_dmg->Fire(p_dmg, 50,
						{ deadPos.x,deadPos.y,deadPos.z },
						7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 8, { 4.0f, 0.0f });
				}
			}

			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				if (wEnemys->GetIsDead() == true)
				{
					Vector3 deadPos{};
					deadPos = wEnemys->GetPosition();
					pm_dmg->Fire(p_dmg, 50,
						{ deadPos.x,deadPos.y,deadPos.z },
						7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 8, { 4.0f, 0.0f });
				}
			}
			pm_dmg->Update();
		}
		else
		{
			player->BulletUpdate();
		}
	}
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
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		enemys->Draw(viewProjection);
		enemys->BulletDraw(viewProjection);
	}

	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		wEnemys->Draw(viewProjection);
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

void GameScene::LoadEnemyPop()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルと閉じる
	file.close();
}

void GameScene::UpdateEnemyPop()
{
	//待機処理
	if (isWait_ == true)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//待機完了
			isWait_ = false;
		}
		return;
	}

	// １行ずつ読み込む
	std::string line;

	while (getline(enemyPopCommands, line))
	{

		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スパース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		std::string word;
		getline(line_stream, word, ' ');

		// 先頭文字列がwEnemyなら頂点座標
		if (key == "wEnemy")
		{
			//敵の生成
			std::unique_ptr<WeakEnemy> newWEnemy = std::make_unique<WeakEnemy>();
			//敵の初期化
			newWEnemy->WEnemyInitialize();
			//コライダーの追加
			newWEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			//移動向き 
			if (word.find("L") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->L);
			}
			else if (word.find("R") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->R);
			}

			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newWEnemy->SetPosition(position);

			newWEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newWEnemy->worldTransform_.UpdateMatrix();
			//登録
			wEnemys_.push_back(std::move(newWEnemy));
		}

		// 先頭文字列がenemyなら頂点座標
		else if (key == "enemy") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy->EnemyInitialize();
			//コライダーの追加
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy->SetPosition(position);
			newEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy->worldTransform_.UpdateMatrix();
			//登録
			enemys_.push_back(std::move(newEnemy));
		}

		else if (key == "wait")
		{
			getline(line_stream, word, ' ');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}

		else if (key == "BOSS")
		{
			isBossScene = true;
			bossCameraPos = viewProjection->GetEye();
		}
	}
}

void GameScene::BossAppears()
{
	if (isBossScene == true)
	{
		bossAppTimer++;
		if (viewProjection->eye.z <= 10)
		{
			viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0, 0.5f));
		}
		if (bossAppTimer >= 180)
		{
			isBossScene = false;
		}
	}
	else if (isBossScene == false && bossAppTimer >= 180)
	{
		if (viewProjection->eye.z > -20)
		{
			viewProjection->SetEye(viewProjection->GetEye() - Vector3(0, 0, 1.0f));
		}
		else
		{
			viewProjection->SetEye(bossCameraPos);
			bossAppTimer = 0;
		}
	}
}
