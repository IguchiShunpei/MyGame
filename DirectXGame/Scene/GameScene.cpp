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

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
	viewProjection->SetTarget(player->GetWorldPosition());

	LoadEnemyPop();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile("backGround");

	//モデル読み込み
	modelMeteor = Model::LoadFromOBJ("meteor");

	meteorModels.insert(std::make_pair("meteor", modelMeteor));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(meteorModels)::iterator it = meteorModels.find(objectData.fileName);
		if (it != meteorModels.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		meteor = new Meteor;
		meteor->MeteorInitialize();
		meteor->SetModel(model);

		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		meteor->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		meteor->SetRotation(rot);

		// 座標
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		meteor->SetScale(scale);

		// 配列に登録
		meteorObjects.push_back(meteor);
	}

	//メンバ変数の初期化
	cameraWorkPos_ = { 0,0,0 };
	//シーン番号
	sceneNum = SceneNum::Title;
	//ゲーム中のシーン番号
	gameNum = GameNum::wEnemyScene;
	//タイトルカメラワーク
	titleNum = 0;
	//ボス
	bossNum_ = 0;
	//墜落量
	gameOverNum_ = 0.0f;

	//フラグ
	isTitleCameraWork_ = false;
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;

	//タイマー
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
}

void GameScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}

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
	switch (sceneNum)
	{
	case Title:
		//天球
		sky->Update();
		viewProjection->UpdateMatrix();

		enemys_.clear();
		wEnemys_.clear();

		if (isTitleCameraWork_ == false)
		{
			if (input->TriggerKey(DIK_SPACE))
			{
				isTitleCameraWork_ = true;
			}
		}
		else
		{
			//タイマーが60になったらtitleNumを+する
			titleTimer_++;
			if (titleTimer_ == 60)
			{
				titleNum++;
				titleTimer_ = 0;
				//カメラの位置を切り替える
				switch (titleNum)
				{
				case 1:
					viewProjection->SetEye(Vector3(0.0f, 0.0f, 10.0f));
					break;
				case 2:
					viewProjection->SetEye(Vector3(0.0f, 2.0f, -20.0f));
					break;
				case 3:
					viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
					viewProjection->SetTarget(Vector3(0.0f, 0.0f, 100.0f));
					break;
				}
			}
			StartCameraWork(titleNum);
			if (titleNum == 4)
			{
				player->SetPosition(Vector3(0, -2, -20));
				viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));
				titleNum = 0;
				titleTimer_ = 0;
				isTitleCameraWork_ = false;
				sceneNum = Game;
			}
		}
		break;
	case Game:
		wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
			{
				return wEnemy->GetIsDead();
			});
		//デスフラグの立った敵を削除
		enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
			{
				return enemy->GetIsDead();
			});

		//更新コマンド
		UpdateEnemyPop();

		//天球
		sky->Update();
		//自機の登場演出
		player->IntitMotion();
		//ボスの登場演出
		BossAppears();


		//カメラ
		viewProjection->UpdateMatrix();

		for (auto& object : meteorObjects)
		{
			object->MeteorUpdate();
		}

		switch (gameNum)
		{
		case wEnemyScene://雑魚敵戦専用の処理

			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Update();
				wEnemys->ColliderUpdate();
			}

			break;

		case BossScene://ボス戦専用の処理

			//ボス登場演出フラグがfalseになったらボス戦開始
			if (isBossScene_ == false)
			{

				//敵
				for (std::unique_ptr<Enemy>& enemys : enemys_)
				{
					enemys->Update();
					enemys->ColliderUpdate();
				}

				//もし攻撃に当たって無かったら
				if (player->GetIsHit() == false)
				{
					cameraWorkPos_ = viewProjection->GetEye();
				}
				else
				{
					hitTimer_++;
					if (hitTimer_ < 8)
					{
						CameraShake();
					}
					else
					{
						//保存していた位置にカメラを戻す
						viewProjection->SetEye(cameraWorkPos_);
						player->SetIsHit(false);
						hitTimer_ = 0;
					}
				}

				//ボス戦で敵の残り数が0になったら
				if (bossNum_ == 0)
				{
					//クリア演出フラグをtrue
					isClearScene_ = true;
				}

				ToClearScene();
				ToGameOverScene();
			}

			break;
		}

		if (player->GetIsInit() == true)
		{

			if (isBossScene_ == false && isClearScene_ == false)
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

				player->Update();
				player->ColliderUpdate();

				//全ての衝突をチェック
				collisionManager->CheckAllCollisions();

				//敵死亡時のパーティクル
				for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
				{
					if (wEnemys->GetIsDead() == true)
					{
						Vector3 deadPos{};
						deadPos = wEnemys->GetPosition();
						pm_dmg->Fire(p_dmg, 50,
							{ deadPos.x,deadPos.y,deadPos.z },
							7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
					}
				}

				for (std::unique_ptr<Enemy>& enemys : enemys_)
				{
					if (enemys->GetIsHit() == true)
					{
						Vector3 hitPos{};
						hitPos = enemys->GetPosition();
						pm_dmg->Fire(p_dmg, 50,
							{ hitPos.x,hitPos.y,hitPos.z },
							7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 4, { 2.0f, 0.0f });
					}
					if (enemys->GetIsDead() == true)
					{
						Vector3 deadPos{};
						deadPos = enemys->GetPosition();
						pm_dmg->Fire(p_dmg, 50,
							{ deadPos.x,deadPos.y,deadPos.z },
							7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
						bossNum_--;
					}
				}

				//パーティクル更新
				pm_dmg->Update();
			}
			else
			{
				//パーティクル更新
				pm_dmg->Update();
				player->BulletUpdate();
			}
		}

		break;
	case Clear:
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
			Reset();
			viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
			viewProjection->SetTarget(player->GetWorldPosition());
		}
		break;
	case GameOver:
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
			Reset();
			viewProjection->SetEye(Vector3(0.0f, 0.0f, 20.0f));
			viewProjection->SetTarget(player->GetWorldPosition());
		}
		break;
	}

}

void GameScene::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

#pragma region 最初のシーンの描画

	Object3d::PreDraw(dxCommon->GetCommandList());

	switch (sceneNum)
	{
	case Title:
		player->Draw(viewProjection);
		sky->Draw(viewProjection);
		break;
	case Game:
		for (auto& object : meteorObjects) {
			object->Draw(viewProjection);
		}
		switch (gameNum)
		{
		case wEnemyScene:
			//雑魚敵
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Draw(viewProjection);
			}
			break;

		case BossScene:
			//ボス敵
			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				if (enemys->GetIsHit() == false)
				{
					enemys->Draw(viewProjection);
				}
				enemys->BulletDraw(viewProjection);
			}
			break;
		}

		sky->Draw(viewProjection);
		player->Draw(viewProjection);
		player->BulletDraw(viewProjection);

		break;
	case Clear:
		break;
	case GameOver:
		break;
	case Pose:
		switch (gameNum)
		{
		case wEnemyScene:
			//雑魚敵
			for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
			{
				wEnemys->Draw(viewProjection);
			}
			break;

		case BossScene:
			//ボス敵
			for (std::unique_ptr<Enemy>& enemys : enemys_)
			{
				if (enemys->GetIsHit() == false)
				{
					enemys->Draw(viewProjection);
				}
				enemys->BulletDraw(viewProjection);
			}
			break;
		}

		sky->Draw(viewProjection);
		player->Draw(viewProjection);
		player->BulletDraw(viewProjection);

		break;
	}

	Object3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	pm_dmg->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

#pragma endregion 最初のシーンの描画

	// 描画後処理
	dxCommon->PostDraw();
}

void GameScene::LoadEnemyPop()
{
	enemys_.clear();
	wEnemys_.clear();

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
		waitTimer_--;
		if (waitTimer_ <= 0)
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

		// wEnemyを読み取って座標をセットする
		if (key == "wEnemy")
		{
			//敵の生成
			std::unique_ptr<WeakEnemy> newWEnemy = std::make_unique<WeakEnemy>();
			//敵の初期化
			newWEnemy->WEnemyInitialize();
			//コライダーの追加
			newWEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			//移動向きを読み取ってセットする
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

		// enemyを読み取って座標をセットする
		else if (key == "enemy") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy->EnemyInitialize();
			//コライダーの追加
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			if (word.find("HP") == 0)
			{
				std::string num;
				getline(line_stream, num, ' ');
				//hpを保存
				int32_t hpNum = atoi(num.c_str());
				newEnemy->SetHp(hpNum);
			}
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
			bossNum_++;
		}
		//待機時間を読み取る
		else if (key == "wait")
		{
			getline(line_stream, word, ' ');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}
		//"BOSS"を読み取ってボス戦に移行
		else if (key == "BOSS")
		{
			isBossScene_ = true;
			gameNum = GameNum::BossScene;
			cameraWorkPos_ = viewProjection->GetEye();
		}
	}
}

void GameScene::BossAppears()
{
	//接近するカメラワーク
	if (isBossScene_ == true)
	{
		if (viewProjection->eye.z <= 10)
		{
			viewProjection->SetEye(viewProjection->GetEye() + Vector3(0, 0, 10) * MathFunc::easeInSine(bossAppTimer_ / 180.0f));
		}
		if (bossAppTimer_ >= 180)
		{
			isBossScene_ = false;
		}
		bossAppTimer_++;
	}
	//引くカメラワーク
	else if (isBossScene_ == false && bossAppTimer_ >= 180)
	{
		if (viewProjection->eye.z > -20)
		{
			viewProjection->SetEye(viewProjection->GetEye() - Vector3(0, 0, 1.0f));
		}
		else
		{
			viewProjection->SetEye(cameraWorkPos_);
			bossAppTimer_ = 0;
		}
	}
}

void GameScene::ToClearScene()
{
	if (isClearScene_ == true)
	{
		//自機を動かす
		player->worldTransform_.position_.z++;
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		//自機が移動しきったらシーン遷移
		if (player->worldTransform_.position_.z >= 100)
		{
			sceneNum = SceneNum::Clear;
			isClearScene_ = false;
		}
	}
}

void GameScene::ToGameOverScene()
{
	if (player->GetIsDead() == true)
	{
		CameraShake();
		//自機を動かす
		player->worldTransform_.position_.y -= 0.05f;
		gameOverNum_ += 0.05f;
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		if (gameOverNum_ >= 3.0f)
		{
			sceneNum = SceneNum::GameOver;
		}
	}
}

void GameScene::CameraShake()
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-0.3f, 0.3f);
	std::uniform_real_distribution<float>dist2(-0.3f, 0.3f);

	viewProjection->eye = viewProjection->eye + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection->UpdateMatrix();
}

void GameScene::Reset()
{
	delete enemy;
	delete wEnemy;

	viewProjection->Initialize();
	viewProjection->SetEye(Vector3(0.0f, 0.0f, -20.0f));

	LoadEnemyPop();

	//player 
	player = new Player;
	player->PlayerInitialize();

	//メンバ変数の初期化
	cameraWorkPos_ = { 0,0,0 };
	//シーン番号
	sceneNum = SceneNum::Title;
	//ゲーム中のシーン番号
	gameNum = GameNum::wEnemyScene;
	//タイトルカメラワーク
	titleNum = 0;
	//ボス
	bossNum_ = 0;
	//墜落量
	gameOverNum_ = 0.0f;

	//フラグ
	isTitleCameraWork_ = false;
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;


	//タイマー
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;

}

void GameScene::StartCameraWork(int num)
{
	switch (num)
	{
	case 0:
		viewProjection->eye.x = 15.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		viewProjection->eye.y = 10.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 1:
		viewProjection->eye.x = 20.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 2:
		viewProjection->eye.z = 30.0f * MathFunc::easeOutSine(titleTimer_ / 60.0f);
		break;
	case 3:
		//自機を動かす
		player->worldTransform_.position_.z++;
		viewProjection->eye.z -= 1.5f;
		player->worldTransform_.rotation_.z = 360.0f * -MathFunc::easeOutSine(titleTimer_ / 60.0f);
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		break;
	}
}
