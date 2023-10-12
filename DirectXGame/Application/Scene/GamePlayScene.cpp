#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <string.h>
#include <math.h>


using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();
	viewProjection->Initialize();

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//player 
	player = new Player;
	player->PlayerInitialize();

	LoadEnemyPop();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect01.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	p_eDmg = Particle::LoadParticleTexture("effect02.png");
	pm_eDmg = ParticleManager::Create();
	pm_eDmg->SetParticleModel(p_eDmg);
	pm_eDmg->SetXMViewProjection(xmViewProjection);

	p_bDmg = Particle::LoadParticleTexture("effect03.png");
	pm_bDmg = ParticleManager::Create();
	pm_bDmg->SetParticleModel(p_bDmg);
	pm_bDmg->SetXMViewProjection(xmViewProjection);

	LoadLevelData();

	//メンバ変数の初期化
	cameraWorkPos_ = { 0,0,0 };
	//ゲーム中のシーン番号
	gameNum = GameNum::FirstScene;
	//墜落量
	gameOverNum_ = 0;
	//シェイク範囲
	shakeRange_ = 3.0f;
	//シェイク移動量
	shakeNum_ = 0.1f;
	//ボスの墜落スピード
	bossDownSpeed_ = 0.01f;
	//ボスalpha
	bossAlpha_ = 1.0f;

	//フラグ
	isBossScene_ = false;
	isWait_ = false;
	isClearScene_ = false;
	isBossEffect_ = false;
	isBossEnemyDead_ = false;
	isBossinit_ = false;
	bossShake_ = true;
	isBossAlpha_ = true;

	//タイマー
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
}

void GamePlayScene::Update()
{
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDead();
		});
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDelete();
		});
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	invEnemys_.remove_if([](std::unique_ptr <InvEnemy>& invEnemy)
		{
			return invEnemy->GetIsDelete();
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
	case FirstScene://雑魚敵戦専用の処理

		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->ColliderUpdate();
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
		{
			invEnemys->Update();
			invEnemys->ColliderUpdate();
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
				if (isBossEnemyDead_ == true)
				{
					enemys->SetIsDead(true);
				}
			}
			//更新
			if (bEnemy->GetIsDeathTimer() == false)
			{
				bEnemy->Update();
				bEnemy->ColliderUpdate();
			}
			else
			{
				isBossEnemyDead_ = true;
				//タイマーを動かす処理
				bEnemy->ActiveDeathTimer();
				//死亡演出
				BossDead();
				if (isClearScene_ == false)
				{
					CameraShake(0.3f,0.3f);
				}
			}
			//死亡フラグがtrueになったら
			if (bEnemy->GetIsDead() == true)
			{
				viewProjection->SetEye(cameraWorkPos_);
				isClearScene_ = true;
			}
			else
			{
				cameraWorkPos_ = viewProjection->GetEye();
			}
			//クリア演出の処理
			ToClearScene();
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
			viewProjection->eye_.x = max(viewProjection->eye_.x, -kCameraLimitX);
			viewProjection->eye_.x = min(viewProjection->eye_.x, +kCameraLimitX);
			viewProjection->eye_.y = max(viewProjection->eye_.y, -kCameraLimitY);
			viewProjection->eye_.y = min(viewProjection->eye_.y, +kCameraLimitY);

			player->Update();
			player->ColliderUpdate();

			//パーティクル更新
			pm_dmg->Update();
			pm_eDmg->Update();
			pm_bDmg->Update();
		}
		else
		{
			//パーティクル更新
			pm_dmg->Update();
			pm_eDmg->Update();
			pm_bDmg->Update();
			player->BulletUpdate();
		}
	}

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	//もし攻撃に当たって無かったら
	if (player->GetIsDead() == false)
	{
		if (player->GetIsHit() == false)
		{
			cameraWorkPos_ = viewProjection->GetEye();
		}
		else
		{
			//攻撃を受けた時の無敵フラグがtrueになったら
			if (player->GetIsInv() == true)
			{
				//無敵時間
				hitTimer_++;
				if (hitTimer_ < 16)
				{
					CameraShake(0.3f,0.3f);
				}
				else
				{
					player->SetIsHit(false);
					player->SetIsInv(false);
					hitTimer_ = 0;
				}
			}
		}
	}

	//雑魚敵死亡時のパーティクル
	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		if (wEnemys->GetIsDead() == true)
		{
			Vector3 deadPos{};
			deadPos = wEnemys->GetPosition();
			pm_eDmg->Fire(p_eDmg, 50,
				{ deadPos.x,deadPos.y,deadPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
		}
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
	{
		if (invEnemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = invEnemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ hitPos.x,hitPos.y,hitPos.z },
				4.0f, 4.0f, 4.0f, 4.0f, 0, 0, 0, 0, 0.2f, 0.1f, 0, 0, 0, 4, { 2.0f, 0.0f });
		}
	}

	//敵の被ダメージ処理
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		if (enemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = enemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ hitPos.x,hitPos.y,hitPos.z },
				4.0f, 4.0f, 4.0f, 4.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 4, { 2.0f, 0.0f });
		}
		if (enemys->GetIsDead() == true)
		{
			Vector3 deadPos{};
			deadPos = enemys->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ deadPos.x,deadPos.y,deadPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 12, { 4.0f, 0.0f });
		}
	}
	//ボス敵の被ダメージ処理
	if (isBossinit_ == true)
	{
		if (bEnemy->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = bEnemy->GetPosition();
			pm_dmg->Fire(p_dmg, 50,
				{ hitPos.x,hitPos.y,hitPos.z },
				7.0f, 7.0f, 7.0f, 7.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 4, { 2.0f, 0.0f });
		}
		if (isBossEnemyDead_ == true)
		{
			if (bEnemy->GetIsDead() == false)
			{
				Vector3 deadPos{};
				deadPos = bEnemy->GetPosition();
				pm_bDmg->Fire(p_bDmg, 50,
					{ deadPos.x,deadPos.y,deadPos.z },
					14.0f, 14.0f, 14.0f, 14.0f, 0, 0, 0, 0, 0.2f, 0.5f, 0, 0, 0, 4, { 4.0f, 0.0f });
			}
		}
	}
	ToGameOverScene();
}

void GamePlayScene::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

#pragma region 最初のシーンの描画

	Object3d::PreDraw(dxCommon->GetCommandList());

	switch (gameNum)
	{
	case FirstScene:
		//雑魚敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Draw(viewProjection);
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
		{
			invEnemys->Draw(viewProjection);
		}
		break;

	case BossScene:
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			if (enemys->GetIsHit() == false)
			{
				enemys->Draw(viewProjection);
			}
			enemys->BulletDraw(viewProjection);
		}
		//ボス敵
		if (isBossinit_ == true)
		{
			if (bEnemy->GetIsDead() == false)
			{
				if (bEnemy->GetIsHit() == false)
				{
					bEnemy->Draw(viewProjection, bossAlpha_);
				}
			}
		}
		break;
	}

	sky->Draw(viewProjection);
	//カメラシェイク中は点滅
	if (hitTimer_ % 2 != 1)
	{
		player->Draw(viewProjection);
	}
	player->BulletDraw(viewProjection);

	for (auto& object : meteorObjects) {
		object->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	pm_dmg->Draw();
	pm_eDmg->Draw();
	pm_bDmg->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

#pragma endregion 最初のシーンの描画

	// 描画後処理
	dxCommon->PostDraw();
}

void GamePlayScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}

	delete sky;
	delete player;
	delete enemy;
	delete wEnemy;
	delete bEnemy;
	delete invEnemy;
	delete p_dmg;
	delete pm_dmg;

	//カメラ解放
	delete viewProjection;
	delete xmViewProjection;
}

void GamePlayScene::GameReset()
{
	viewProjection->Initialize();
}

void GamePlayScene::LoadEnemyPop()
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

void GamePlayScene::UpdateEnemyPop()
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

		// wEnemy
		if (key == "wEnemy")
		{
			std::string word;
			getline(line_stream, word, ' ');
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
		// enemy
		else if (key == "enemy") {
			std::string word;
			getline(line_stream, word, ' ');
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
		}
		// invEnemy
		else if (key == "invEnemy") {
			//敵の生成
			std::unique_ptr<InvEnemy> newInvEnemy = std::make_unique<InvEnemy>();
			//敵の初期化
			newInvEnemy->InvEnemyInitialize();
			//コライダーの追加
			newInvEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newInvEnemy->SetPosition(position);
			newInvEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newInvEnemy->worldTransform_.UpdateMatrix();
			//登録
			invEnemys_.push_back(std::move(newInvEnemy));
		}
		// bossEnemy
		else if (key == "bossEnemy") {
			std::string word;
			getline(line_stream, word, ' ');
			//敵の生成
			bEnemy = new BossEnemy;
			//敵の初期化
			bEnemy->BossEnemyInitialize();
			//コライダーの追加
			bEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			if (word.find("HP") == 0)
			{
				std::string num;
				getline(line_stream, num, ' ');
				//hpを保存
				int32_t hpNum = atoi(num.c_str());
				bEnemy->SetHp(hpNum);
			}
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			bEnemy->SetPosition(position);
			bEnemy->SetScale(Vector3(1.0f, 1.0f, 1.0f));
			bEnemy->worldTransform_.UpdateMatrix();
			isBossinit_ = true;
		}
		//待機時間を読み取る
		else if (key == "wait")
		{
			std::string word;
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

void GamePlayScene::BossAppears()
{
	//接近するカメラワーク
	if (isBossScene_ == true)
	{
		if (viewProjection->eye_.z <= 10)
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
		if (viewProjection->eye_.z > -20)
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
void GamePlayScene::BossDead()
{
	//墜落
	bEnemy->worldTransform_.position_.y -= bossDownSpeed_;
	//左右にシェイク
	if (bossShake_ == true)
	{
		if (bEnemy->worldTransform_.position_.x <= shakeRange_)
		{
			bEnemy->worldTransform_.position_.x += shakeNum_;
		}
		else
		{
			bossShake_ = false;
		}
	}
	else
	{
		if (bEnemy->worldTransform_.position_.x >= -shakeRange_)
		{
			bEnemy->worldTransform_.position_.x -= shakeNum_;
		}
		else
		{
			bossShake_ = true;
		}
	}
	//alpha値を変化
	if (isBossAlpha_ == true)
	{
		if (bossAlpha_ <= 1.0f)
		{
			bossAlpha_ += 0.05f;
		}
		else
		{
			isBossAlpha_ = false;
		}
	}
	else
	{
		if (bossAlpha_ >= 0.5f)
		{
			bossAlpha_ -= 0.05f;
		}
		else
		{
			isBossAlpha_ = true;
		}
	}

	bEnemy->worldTransform_.UpdateMatrix();
}
void GamePlayScene::ToClearScene()
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

			GameSceneManager::GetInstance()->ChangeScene("CLEAR");
			isClearScene_ = false;
		}
	}
}
void GamePlayScene::ToGameOverScene()
{
	if (player->GetIsDead() == true)
	{
		CameraShake(0.3f,0.3f);
		//自機を動かす
		player->worldTransform_.position_.y -= 0.05f;
		gameOverNum_++;
		// ワールドトランスフォームの行列更新と転送
		player->worldTransform_.UpdateMatrix();
		if (gameOverNum_ >= 60)
		{
			GameSceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}
}
void GamePlayScene::CameraShake(float x,float y)
{
	viewProjection->eye_ = cameraWorkPos_;
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-x, x);
	std::uniform_real_distribution<float>dist2(-y, y);

	viewProjection->eye_ = viewProjection->eye_ + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection->UpdateMatrix();
}

void GamePlayScene::LoadLevelData()
{
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
		//初期Y座標を保存しておく
		meteor->SetBeforeY(pos.y);

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
}
