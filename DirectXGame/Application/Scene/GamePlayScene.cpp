/**
* @file GamePlayScene.cpp
* @brief ゲームのメイン部分
* @author イグチ_シュンペイ
*/

#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Vector3.h"
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
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//当たり判定
	collisionManager_ = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->SetEye({ -15.0f,0.0f,10.0f });
	viewProjection_->SetUp({ 0.0f,1.0f,0.0f });
	startCameraPos_ = viewProjection_->GetEye();
	startCamoreMovePos_ = { 15.0f,5.0f,30.0f };
	beforeTargetNum_ = viewProjection_->target_;
	normalTargetNum_ = viewProjection_->target_;
	normalUpNum_ = viewProjection_->up_;

	//天球
	sky_ = std::make_unique < SkyDome>();
	sky_->SkyDomeInitialize();
	sky_->SetPosition({ 0.0f,0.0f,500.0f });

	//爆発
	explosion01_ = std::make_unique<Explosion>();
	explosion01_->ExplosionInitialize(0);
	explosion02_ = std::make_unique<Explosion>();
	explosion02_->ExplosionInitialize(1);
	explosion03_ = std::make_unique<Explosion>();
	explosion03_->ExplosionInitialize(2);

	//player 
	player = std::make_unique <Player>();
	player->PlayerInitialize();

	//bossEnemy
	bEnemy = std::make_unique < BossEnemy>();
	bEnemy->BossEnemyInitialize();
	bEnemy->worldTransform_.UpdateMatrix();
	bEnemy->ColliderUpdate();

	//敵ファイル読み込み
	LoadEnemyPop();

	//パーティクル
	//ヒットテクスチャ
	p_Hit.reset(Particle::LoadParticleTexture("effect01.png"));
	pm_Hit = ParticleManager::Create();
	pm_Hit->SetParticleModel(p_Hit.get());
	pm_Hit->SetViewProjection(viewProjection_.get());
	//wEnemyテクスチャ
	p_WDmg.reset(Particle::LoadParticleTexture("effect02.png"));
	pm_WDmg = ParticleManager::Create();
	pm_WDmg->SetParticleModel(p_WDmg.get());
	pm_WDmg->SetViewProjection(viewProjection_.get());
	//bEnemyテクスチャ
	p_BDmg.reset(Particle::LoadParticleTexture("effect03.png"));
	pm_BDmg = ParticleManager::Create();
	pm_BDmg->SetParticleModel(p_BDmg.get());
	pm_BDmg->SetViewProjection(viewProjection_.get());
	//meteorテクスチャ
	p_Meteor.reset(Particle::LoadParticleTexture("effect04.png"));
	pm_Meteor = ParticleManager::Create();
	pm_Meteor->SetParticleModel(p_Meteor.get());
	pm_Meteor->SetViewProjection(viewProjection_.get());
	//基本爆発テクスチャ
	p_Ex.reset(Particle::LoadParticleTexture("effect05.png"));
	pm_Ex = ParticleManager::Create();
	pm_Ex->SetParticleModel(p_Ex.get());
	pm_Ex->SetViewProjection(viewProjection_.get());
	//自機爆発テクスチャ
	p_PEx.reset(Particle::LoadParticleTexture("effect06.png"));
	pm_PEx = ParticleManager::Create();
	pm_PEx->SetParticleModel(p_PEx.get());
	pm_PEx->SetViewProjection(viewProjection_.get());
	//自機爆発テクスチャ
	p_Smoke.reset(Particle::LoadParticleTexture("effect07.png"));
	pm_Smoke = ParticleManager::Create();
	pm_Smoke->SetParticleModel(p_Smoke.get());
	pm_Smoke->SetViewProjection(viewProjection_.get());

	//UIの初期化
	ui_ = std::make_unique<UI>();
	ui_->UIInitialize();

	//レベルデータのロード
	LoadLevelData();

	//メンバ変数の初期化
	cameraShakePos_ = { 0.0f,0.0f,0.0f };
	deadCameraPos_ = { 0.0f,0.0f,0.0f };
	deadCameraMovePos_ = { 20.0f ,0.0f,35.0f };
	bossInitCameraPos_ = { 0.0f,0.0f,0.0f };

	//基本target
	changeTargetNum_ = { 0.0f,0.0f,0.0f };
	//登場した後にtargetを戻す値
	changeTargetMoveNum_ = { 0.0f,0.0f,50.0f };

	//登場した後の各座標
	afterInitCameraPos_ = { 0.0f, 5.0f, -20.0f };
	afterInitCameraTarget_ = { 0.0f, -2.0f, 50.0f };

	allZero_ = { 0.0f,0.0f,0.0f };

	//ゲーム中のシーン番号
	gameNum_ = GameNum::FirstScene;
	//カメラワーク番号
	bossInitNum_ = BossInitNum::Up;

	//ゲームオーバーシーンに遷移する時間
	gameOverTimer_ = 0;
	//最大
	gameOverTimerMax_ = 60;

	clearCameraNum_ = 0;
	gameClearmoveZ_ = 0.5f;
	gameClearPosZ_ = 100.0f;
	clearCameraMoveZ_ = 0.05f;

	//シェイク
	playerCameraShake_ = 0.2f;
	bossCameraShake_ = 1.5f;
	enemyCameraShake_ = 0.5f;

	//ボス登場
	upZ_ = 10.0f;
	looseZ_ = 1.0f;
	blackAlphaMax_ = 1.0f;

	//フラグ
	isWait_ = false;
	isClearScene_ = false;
	isBossEffect_ = false;
	isBEnemyDeadScene_ = false;
	isBossInitCamera_ = false;
	isStart_ = false;
	isDeadCameraShake_ = false;

	//タイマー
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	bossAppTimerMax_ = 180.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
	bossInitTimer_ = 0;
	bossInitTimerMax_ = 24;
	startTimer_ = 0.0f;
	startTimerMax_ = 120.0f;
	cameraMoveTimer_ = 0.0f;
	cameraMoveTimerMax_ = 60.0f;
	playerDeadTimer_ = 0.0f;
	playerDeadTimerMax_ = 60.0f;
	targetMoveTimer_ = 0.0f;
	targetMoveTimerMax_ = cameraMoveTimerMax_ * 2.0f;
	hitPlayerTimer_ = 0;
	hitPlayerTimerMax_ = 32;
	hitEnemyTimer_ = 0;
	clearCameraTimer_ = 0.0f;
	clearCameraTimerMax_ = 180.0f;
	playerShakeTimer_ = 0;
}

void GamePlayScene::Update()
{
	DeleteObject();

	//更新コマンド
	UpdateEnemyPop();

	//天球
	sky_->worldTransform_.UpdateMatrix();

	//黒フェードアウト
	ui_->FadeOut(ui_->Black);

	//自機登場演出
	PlayerInit();

	for (auto& object : stardustObjects_)
	{
		object->StardustUpdate();
	}
	for (auto& object : meteorObjects_)
	{
		object->MeteorUpdate();
	}

	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		meteors->MeteorUpdate();
		if (meteors->GetIsDead() == true)
		{
			//敵の生成
			std::unique_ptr<Item> newItem = std::make_unique<Item>();
			//敵の初期化
			newItem->ItemInitialize(meteors->GetPosition());
			//コライダーの追加
			newItem->SetCollider(new SphereCollider(Vector3(0, 0, 0), 2.5f));
			//登録
			items_.push_back(std::move(newItem));
		}
	}

	for (std::unique_ptr<Item>& items : items_)
	{
		items->ItemUpdate();
		items->ColliderUpdate();
		if (items->GetIsHit() == true)
		{
			if (player->GetBulletLevel() < 2)
			{
				player->SetBulletLevel(player->GetBulletLevel() + 1);
			}
		}
	}

	switch (gameNum_)
	{
	case FirstScene://雑魚敵戦専用の処理

		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->SetDamage(player->GetBulletPower());
			wEnemys->ColliderUpdate();
		}
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Update(player->GetPosition());
			invEnemys->ColliderUpdate();
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Update(player->GetPosition());
			enemys->SetDamage(player->GetBulletPower());
			enemys->ColliderUpdate();
		}
		break;

	case BossScene://ボス戦専用の処理
		//ボスの登場演出
		BossInit();
		//ボス登場演出フラグがfalseになったらボス戦開始
		if (isBossInitCamera_ == false)
		{
			//更新
			if (bEnemy->GetIsDeathTimer() == false)
			{
				bEnemy->Update();
				bEnemy->SetDamage(player->GetBulletPower());
				bEnemy->PhaseChange(player->GetPosition());
				bEnemy->ColliderUpdate();
			}
			else
			{
				isBEnemyDeadScene_ = true;
				//タイマーを動かす処理
				bEnemy->ActiveDeathTimer();
				//死亡演出
				BossDead();
				if (isClearScene_ == false)
				{
					//deathTimerが奇数なら
					if (bEnemy->GetDeathTimer() % 2 != 1)
					{
						CameraShake(enemyCameraShake_, enemyCameraShake_);
					}
					else
					{
						cameraShakePos_ = viewProjection_->GetEye();
					}
				}
			}
			if (isClearScene_ == false)
			{
				//死亡フラグがtrueになったら
				if (bEnemy->GetIsDead() == true)
				{
					viewProjection_->SetEye(cameraShakePos_);
					isClearScene_ = true;
					normalEyeNum_ = viewProjection_->eye_;
					normalTargetNum_ = viewProjection_->target_;
					viewProjection_->SetTarget(player->GetPosition());
				}
				else
				{
					cameraShakePos_ = viewProjection_->GetEye();
				}
			}
			//クリア演出の処理
			ToClearScene();
		}
		break;
	}

	if (isPlayerInit_ == true)
	{
		//UIの登場モーション
		ui_->UIInitMotion();
		if (isBossInitCamera_ == false && isClearScene_ == false && isBEnemyDeadScene_ == false)
		{
			MoveCamera();

			player->Update();
			player->ColliderUpdate();
		}
		else
		{
			player->BulletUpdate();
			bEnemy->BulletUpdate();
		}
		//UI更新
		ui_->UIUpdate();
		//パーティクル更新
		pm_Hit->Update();
		pm_WDmg->Update();
		pm_BDmg->Update();
		pm_Meteor->Update();
		pm_Ex->Update();
		pm_PEx->Update();
		pm_Smoke->Update();
	}

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	//もし攻撃に当たって無かったら
	if (player->GetIsDead() == false)
	{
		if (player->GetIsHit() == false)
		{
			cameraShakePos_ = viewProjection_->GetEye();
		}
		else
		{
			//攻撃を受けた時の無敵フラグがtrueになったら
			if (player->GetIsInv() == true)
			{
				if (hitPlayerTimer_ <= hitPlayerTimerMax_)
				{
					ui_->SetIsRed(true);
					ui_->FadeOut(ui_->Red);
					if (hitPlayerTimer_ % 2 != 1)
					{
						CameraShake(playerCameraShake_, playerCameraShake_);
					}
					else
					{
						viewProjection_->SetEye(cameraShakePos_);
					}
					ui_->Damage((float)hitPlayerTimer_, (float)hitPlayerTimerMax_, player->GetHp(), player->GetHpMax());
					//無敵時間
					hitPlayerTimer_++;
				}
				else
				{
					ui_->SetIsRed(false);
					ui_->RedReset();
					player->SetIsHit(false);
					player->SetIsInv(false);
					viewProjection_->SetEye(cameraShakePos_);
					hitPlayerTimer_ = 0;
				}
			}
		}
	}

	//雑魚敵死亡時のパーティクル
	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		if (wEnemys->GetIsDead() == true)
		{
			isDeadCameraShake_ = true;
			Vector3 deadPos{};
			deadPos = wEnemys->GetPosition();
			pm_WDmg->Fire(p_WDmg.get(), 40, deadPos, 12, false, {1.0f, 0.0f});
			pm_Ex->Fire(p_Ex.get(), 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex.get(), 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke.get(), 30, deadPos, 5, false, { 5.0f, 0.0f });
		}
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
	{
		if (invEnemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = invEnemys->GetPosition();
			pm_Hit->Fire(p_Hit.get(), 40, hitPos, 5, false, { 1.0f, 0.0f });
		}
	}

	//敵の被ダメージ処理
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		if (enemys->GetIsDead() == true)
		{
			isDeadCameraShake_ = true;
			Vector3 deadPos{};
			deadPos = enemys->GetPosition();
			pm_Meteor->Fire(p_Meteor.get(), 40, deadPos, 12, false, { 1.0f, 0.0f });
			pm_Ex->Fire(p_Ex.get(), 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex.get(), 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke.get(), 30, deadPos, 5, false, { 5.0f, 0.0f });
		}
	}

	//隕石のパーティクル
	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		if (meteors->GetIsDead() == true)
		{
			isDeadCameraShake_ = true;
			Vector3 deadPos{};
			deadPos = meteors->GetPosition();
			pm_Meteor->Fire(p_Meteor.get(), 40, deadPos, 12, false, { 1.0f, 0.0f });
			pm_Ex->Fire(p_Ex.get(), 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex.get(), 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke.get(), 30, deadPos, 5, false, { 5.0f, 0.0f });
		}
	}

	//敵に弾が当たった時のカメラシェイク
	if (isDeadCameraShake_ == true)
	{
		hitEnemyTimer_++;
		if (hitEnemyTimer_ < 16)
		{
			if (hitEnemyTimer_ % 2 != 1)
			{
				CameraShake(enemyCameraShake_, enemyCameraShake_);
			}
			else
			{
				viewProjection_->SetEye(cameraShakePos_);
			}
		}
		else
		{
			hitEnemyTimer_ = 0;
			isDeadCameraShake_ = false;
		}
	}
	else
	{
		cameraShakePos_ = viewProjection_->GetEye();
	}
	//ボス敵の被ダメージ処理
	if (bEnemy->GetIsInit() == true)
	{
		if (isBEnemyDeadScene_ == true)
		{
			if (bEnemy->GetIsDead() == false)
			{
				bossDeadPos_ = bEnemy->GetPosition();
				explosion01_->SetPosition(Vector3(bossDeadPos_.x, bossDeadPos_.y, bossDeadPos_.z));
				explosion02_->SetPosition(Vector3(bossDeadPos_.x, bossDeadPos_.y, bossDeadPos_.z));
				pm_Ex->Fire(p_Ex.get(), 20, bossDeadPos_, 4, false, { 8.0f, 0.0f });
			}
		}
	}
	ToGameOverScene();

	//カメラ
	viewProjection_->UpdateMatrix();
}

void GamePlayScene::Draw()
{
	//描画前処理
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky_->Draw(viewProjection_.get());
	if (isPlayerDead_ == false)
	{
		player->Draw(viewProjection_.get());
		player->BulletDraw(viewProjection_.get());
	}

	//星屑
	for (auto& object : stardustObjects_) {
		object->Draw(viewProjection_.get(), 1.0f, object->GetColor());
	}
	//隕石
	for (auto& object : meteorObjects_) {
		object->Draw(viewProjection_.get(), object->GetAlpha(), object->GetColor());
	}

	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		if (meteors->GetIsDelete() == false)
		{
			meteors->Draw(viewProjection_.get(), meteors->GetAlpha(), meteors->GetColor());
		}
	}
	for (std::unique_ptr<Item>& items : items_)
	{
		items->Draw(viewProjection_.get());
	}
	switch (gameNum_)
	{
	case FirstScene:
		//雑魚敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Draw(viewProjection_.get(), 1.0f, wEnemys->GetColor());
		}
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Draw(viewProjection_.get());
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Draw(viewProjection_.get(), 1.0f, enemys->GetColor());

			enemys->BulletDraw(viewProjection_.get());
		}
		break;

	case BossScene:
		//ボス敵
		if (bEnemy->GetIsDead() == false)
		{
			bEnemy->Draw(viewProjection_.get(), bEnemy->GetAlpha(), bEnemy->GetColor());
			bEnemy->BulletDraw(viewProjection_.get());
		}
		break;
	}

	if (isBEnemyDeadScene_ == true)
	{
		explosion01_->ExplosionDraw(viewProjection_.get());
		explosion02_->ExplosionDraw(viewProjection_.get());
	}

	if (isPlayerDead_ == true && explosion03_->GetIsFinish() == false)
	{
		explosion03_->ExplosionDraw(viewProjection_.get());
	}

	Object3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon_->GetCommandList());

	pm_Hit->Draw();
	pm_WDmg->Draw();
	pm_BDmg->Draw();
	pm_Meteor->Draw();
	pm_Ex->Draw();
	pm_PEx->Draw();
	pm_Smoke->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

	ui_->UIDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void GamePlayScene::Finalize()
{

}

void GamePlayScene::LoadEnemyPop()
{
	enemys_.clear();
	wEnemys_.clear();
	invincibleEnemys_.clear();

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

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

	while (getline(enemyPopCommands_, line))
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
			newWEnemy->SetBeforeY(position.y);
			position.y += 20.0f;
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
			newEnemy->SetBeforeY(position.y);
			position.y += 20.0f;
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
			std::unique_ptr<InvincibleEnemy> newInvincibleEnemy = std::make_unique<InvincibleEnemy>();
			//敵の初期化
			newInvincibleEnemy->InvincibleEnemyInitialize();
			//コライダーの追加
			newInvincibleEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			newInvincibleEnemy->SetBeforeY(position.y);
			position.y += 20.0f;
			// 座標データに追加
			newInvincibleEnemy->SetPosition(position);
			newInvincibleEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newInvincibleEnemy->worldTransform_.UpdateMatrix();
			//登録
			invincibleEnemys_.push_back(std::move(newInvincibleEnemy));
		}
		//meteor
		else if (key == "meteor") {
			std::string word;
			getline(line_stream, word, ' ');
			//敵の生成
			std::unique_ptr<Meteor> newMeteor = std::make_unique<Meteor>();
			//敵の初期化
			newMeteor->MeteorInitialize();
			//コライダーの追加
			newMeteor->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			if (word.find("HP") == 0)
			{
				std::string num;
				getline(line_stream, num, ' ');
				//hpを保存
				int32_t hpNum = atoi(num.c_str());
				newMeteor->SetHp(hpNum);
			}
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newMeteor->SetPosition(position);
			newMeteor->worldTransform_.UpdateMatrix();
			//登録
			meteors_.push_back(std::move(newMeteor));
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
			isBossInitCamera_ = true;
			gameNum_ = GameNum::BossScene;
			cameraShakePos_ = viewProjection_->GetEye();
		}
	}
}

void GamePlayScene::DeleteObject()
{
	//デスフラグの立った敵を削除
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDead();
		});
	wEnemys_.remove_if([](std::unique_ptr <WeakEnemy>& wEnemy)
		{
			return wEnemy->GetIsDelete();
		});
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDelete();
		});
	invincibleEnemys_.remove_if([](std::unique_ptr <InvincibleEnemy>& invEnemy)
		{
			return invEnemy->GetIsDelete();
		});
	items_.remove_if([](std::unique_ptr <Item>& item)
		{
			return item->GetIsDelete();
		});
	meteors_.remove_if([](std::unique_ptr <Meteor>& meteor)
		{
			return meteor->GetIsDelete();
		});
	meteors_.remove_if([](std::unique_ptr <Meteor>& meteor)
		{
			return meteor->GetIsDead();
		});
}

void GamePlayScene::PlayerInit()
{
	if (isPlayerInit_ == false)
	{
		//カメラワーク
		PlayerInitCameraWork();
		//自機の登場モーション
		player->InitMotion();
		//カメラワークが終わったらフラグtrue
		if (isStart_ == true)
		{
			isPlayerInit_ = true;
		}
	}
}

void GamePlayScene::PlayerInitCameraWork()
{
	if (isStart_ == false)
	{
		viewProjection_->SetTarget(player->GetPosition());
		//自機が到着したらカメラを初期位置へ
		if (player->GetIsInit() == true)
		{
			viewProjection_->eye_.x = startCameraPos_.x + startCamoreMovePos_.x * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.y = startCameraPos_.y + startCamoreMovePos_.y * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.z = startCameraPos_.z + startCamoreMovePos_.z * -MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->target_.z = changeTargetNum_.z + changeTargetMoveNum_.z * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			startTimer_++;
			if (startTimer_ > startTimerMax_)
			{
				viewProjection_->SetEye(afterInitCameraPos_);
				viewProjection_->SetTarget(afterInitCameraTarget_);
				beforeEyeNum_ = allZero_;
				beforeTargetNum_ = allZero_;
				beforeUpNum_ = allZero_;
				normalEyeNum_ = viewProjection_->eye_;
				normalTargetNum_ = viewProjection_->target_;
				normalUpNum_ = viewProjection_->up_;
				isStart_ = true;
			}
		}
	}
}

void GamePlayScene::PlayerDead()
{
	if (isPlayerDead_ == false)
	{
		pm_Smoke->PlayerExBefore(p_Smoke.get(), 20, player->GetPosition(), 1, { 2.0f, 0.0f });
		pm_Ex->PlayerExBefore(p_Ex.get(), 20, player->GetPosition(), 2, { 2.0f, 0.0f });
		//自機を動かす
		player->worldTransform_.rotation_.z += 8.0f;
		if (player->worldTransform_.rotation_.z >= 720.0f)
		{
			isPlayerDead_ = true;
			pm_PEx->PlayerExAfter(p_PEx.get(), 200, player->GetPosition(), 12, { 1.0f, 0.0f });
		}
	}
}

void GamePlayScene::BossInit()
{
	if (isBossInitCamera_ == true)
	{
		ui_->Stop();
		BossInitCameraWork();
	}
}
void GamePlayScene::BossInitCameraWork()
{
	switch (bossInitNum_)
	{
	case None:

		break;
	case Up:
		//カメラを寄せる
		if (viewProjection_->eye_.z <= upZ_)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() + Vector3(0, 0, upZ_) * MathFunc::easeInSine(bossAppTimer_ / bossAppTimerMax_));
		}
		else
		{
			//寄ったら黒フェード
			ui_->FadeIn();
			if (ui_->GetIsBlack() == true)
			{
				//黒フェードが完了したら登場カメラワークへ
				bossInitNum_ = InitCameraWork;
				bossAppTimer_ = 0;
				viewProjection_->up_ = { 0.0f,1.0f,0.0f };
			}
		}
		bossAppTimer_++;

		break;
	case InitCameraWork:
		//黒フェードアウト
		ui_->FadeOut(ui_->Black);
		viewProjection_->SetTarget(bEnemy->GetPosition());
		bEnemy->InitMotion();

		if (bEnemy->GetIsInit() == true)
		{
			//登場カメラシェイク時間
			bossInitTimer_++;
			if (bossInitTimer_ < bossInitTimerMax_)
			{
				//bossInitTimerが奇数ならカメラシェイク
				if (bossInitTimer_ % 2 != 1)
				{
					CameraShake(bossCameraShake_, bossCameraShake_);
				}
				else
				{
					viewProjection_->SetEye(bossInitCameraPos_);
				}
			}
			else
			{
				bossInitTimer_ = 0;
				viewProjection_->SetEye(bossInitCameraPos_);
				bossInitNum_ = Loose;
			}
		}
		else
		{
			bossInitCameraPos_ = viewProjection_->GetEye();
		}

		break;
	case Loose:
		if (viewProjection_->eye_.z > afterInitCameraPos_.z)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() - Vector3(0, 0, looseZ_));
		}
		else
		{
			viewProjection_->eye_.z = afterInitCameraPos_.z;
			isBossInitCamera_ = false;
			bossInitNum_ = None;
		}
		break;
	}
}

//↓ボスクラスに移植する
void GamePlayScene::BossDead()
{
	//UI退場
	ui_->UIOutMotion();
	explosion01_->EnemyExplosionUpdate();
	explosion02_->EnemyExplosionUpdate();
	bEnemy->Dead();
}
void GamePlayScene::ToClearScene()
{
	if (isClearScene_ == true)
	{
		if (isClearCameraWork_ == false)
		{
			//カメラワーク
			ToClearCameraWork();
		}
		else
		{
			//黒フェードイン
			ui_->SetIsBlack(false);
			ui_->FadeIn();
			//自機を動かす
			player->worldTransform_.rotation_.z++;
			player->worldTransform_.position_.x -= clearCameraMoveZ_;
			player->worldTransform_.position_.z++;
			viewProjection_->eye_.z -= gameClearmoveZ_;
			// ワールドトランスフォームの行列更新と転送
			player->worldTransform_.UpdateMatrix();
			//自機が移動しきったらシーン遷移
			if (player->worldTransform_.position_.z >= gameClearPosZ_)
			{
				GameSceneManager::GetInstance()->ChangeScene("CLEAR");
				isClearScene_ = false;
			}
		}
	}
}
void GamePlayScene::ToClearCameraWork()
{
	if (clearCameraTimer_ <= clearCameraTimerMax_)
	{
		switch (clearCameraNum_)
		{
			//自機の周りの点を移動
		case 0:
			viewProjection_->eye_.x = normalEyeNum_.x + (20.0f - normalEyeNum_.x) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			viewProjection_->eye_.z = normalEyeNum_.z + (20.0f - normalEyeNum_.z) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			normalTargetNum_ = viewProjection_->target_;
			break;
		case 1:
			viewProjection_->eye_.x = normalEyeNum_.x + (0.0f - normalEyeNum_.x) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			viewProjection_->eye_.z = normalEyeNum_.z + (-20.0f - normalEyeNum_.z) * MathFunc::easeOutSine(clearCameraTimer_ / clearCameraTimerMax_);
			break;
		case 2:
			//1回転したらカメラワークフラグをtrueに
			isClearCameraWork_ = true;
			viewProjection_->eye_.x = 0.0f;
			viewProjection_->eye_.z = -20.0f;
			break;
		}
		clearCameraTimer_++;
		viewProjection_->UpdateMatrix();
	}
	else
	{
		clearCameraTimer_ = 0.0f;
		clearCameraTimerMax_ = 90.0f;
		normalEyeNum_ = viewProjection_->eye_;
		clearCameraNum_++;
	}
}

void GamePlayScene::ToGameOverScene()
{
	if (player->GetIsDead() == true)
	{
		//UI退場
		ui_->UIOutMotion();
		//カメラワーク
		ToGameOverCameraWork();
		//自機死亡演出
		PlayerDead();
		player->worldTransform_.UpdateMatrix();
		if (isGameOver_ == true)
		{
			explosion03_->PlayerExplosionUpdate(player->GetPosition());
			CameraShake(0.1f, 0.0f);
			if (explosion03_->GetIsFinish() == true)
			{
				viewProjection_->eye_ = normalEyeNum_;
				//フェードイン
				ui_->SetIsBlack(false);
				ui_->FadeIn();
				gameOverTimer_++;
				//gameoverシーンへ
				if (gameOverTimer_ >= gameOverTimerMax_)
				{
					GameSceneManager::GetInstance()->ChangeScene("GAMEOVER");
				}
			}
		}
	}
	else
	{
		deadCameraPos_ = viewProjection_->GetEye();
	}
}
void GamePlayScene::ToGameOverCameraWork()
{
	viewProjection_->SetTarget(player->GetPosition());
	if (isPlayerDead_ == false)
	{
		if (playerDeadTimer_ <= playerDeadTimerMax_)
		{
			viewProjection_->eye_.x = deadCameraPos_.x + deadCameraMovePos_.x * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			viewProjection_->eye_.y = deadCameraPos_.y + deadCameraMovePos_.y * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			viewProjection_->eye_.z = deadCameraPos_.z + deadCameraMovePos_.z * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			playerDeadTimer_++;
		}
		else
		{
			normalEyeNum_ = viewProjection_->eye_;
		}
	}
	else
	{
		isGameOver_ = true;
	}

}
void GamePlayScene::CameraShake(float x, float y)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-x, x);
	std::uniform_real_distribution<float>dist2(-y, y);

	viewProjection_->eye_ = viewProjection_->eye_ + Vector3(dist(engine), dist2(engine), dist2(engine));
	viewProjection_->UpdateMatrix();
}

void GamePlayScene::MoveCamera() {

	//カメラ移動処理
	if (input_->PushKey(DIK_UP))
	{
		ui_->SetIsUp(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, 0.1f, 0));
	}
	else
	{
		ui_->SetIsUp(false);
		if (viewProjection_->target_.y >= normalTargetNum_.y)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -0.1f, 0));
		}
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		ui_->SetIsRight(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0.1f, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(-0.0025f, 0, 0));
	}
	else
	{
		ui_->SetIsRight(false);
		if (viewProjection_->target_.x >= normalTargetNum_.x)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-0.1f, 0, 0));
		}
		if (viewProjection_->up_.x <= normalUpNum_.x)
		{
			viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
		}
	}
	if (input_->PushKey(DIK_LEFT))
	{
		ui_->SetIsLeft(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-0.1f, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
	}
	else
	{
		ui_->SetIsLeft(false);
		if (viewProjection_->target_.x <= normalTargetNum_.x)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0.1f, 0, 0));
		}
		if (viewProjection_->up_.x >= normalUpNum_.x)
		{
			viewProjection_->SetUp(viewProjection_->up_ + Vector3(-0.0025f, 0, 0));
		}
	}

	if (input_->PushKey(DIK_DOWN))
	{
		ui_->SetIsDown(true);
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -0.1f, 0));
	}
	else
	{
		ui_->SetIsDown(false);
		if (viewProjection_->target_.y <= normalTargetNum_.y)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, 0.1f, 0));
		}
	}

	//移動限界座標
	const float kTargetLimitX = 9.0f;
	const float kTargetLimitY = 7.0f;

	//範囲を超えない処理
	viewProjection_->target_.x = max(viewProjection_->target_.x, -kTargetLimitX);
	viewProjection_->target_.x = min(viewProjection_->target_.x, +kTargetLimitX);
	viewProjection_->target_.y = max(viewProjection_->target_.y, -kTargetLimitY);
	viewProjection_->target_.y = min(viewProjection_->target_.y, +kTargetLimitY);

	const float kUpLimitX = 0.15f;
	viewProjection_->up_.x = max(viewProjection_->up_.x, -kUpLimitX);
	viewProjection_->up_.x = min(viewProjection_->up_.x, +kUpLimitX);
}

void GamePlayScene::LoadLevelData()
{
	//背景オブジェクトデータの読み込み
	backGroundStar_ = LevelLoader::LoadFile("stardust");
	backGroundMeteor_ = LevelLoader::LoadFile("meteor");

	//モデル読み込み
	std::unique_ptr<Model>backModelStardust = Model::LoadFromOBJ("stardust");
	stardustModels_.insert(std::make_pair("stardust", move(backModelStardust)));
	std::unique_ptr<Model>backModelMeteor = Model::LoadFromOBJ("bigMeteor");
	meteorModels_.insert(std::make_pair("meteor", move(backModelMeteor)));

	// レベルデータからオブジェクトを生成、配置
	//星屑
	for (auto& objectData : backGroundStar_->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(stardustModels_)::iterator it = stardustModels_.find(objectData.fileName);
		if (it != stardustModels_.end()) { model = it->second.get(); }
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Stardust> newObject = std::make_unique<Stardust>();
		newObject->StardustInitialize();
		newObject->SetModel(model);
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		newObject->SetPosition(pos);
		//初期Y座標を保存しておく
		newObject->SetBeforeY(pos.y);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		newObject->SetRotation(rot);

		newObject->SetColor();
		newObject->SetSize();

		// 配列に登録
		stardustObjects_.push_back(move(newObject));
	}
	//隕石
	for (auto& objectData : backGroundMeteor_->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(meteorModels_)::iterator it = meteorModels_.find(objectData.fileName);
		if (it != meteorModels_.end()) { model = it->second.get(); }
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Meteor> newObject = std::make_unique<Meteor>();;
		newObject->MeteorInitialize();
		newObject->SetModel(model);
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		newObject->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		newObject->SetRotation(rot);

		// 大きさ
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		newObject->SetScale(scale);

		newObject->SetIsBack(true);
		newObject->SetAlpha(1.0f);
		newObject->SetSpeed(0.1f, 1.0f);

		// 配列に登録
		meteorObjects_.push_back(move(newObject));
	}
}