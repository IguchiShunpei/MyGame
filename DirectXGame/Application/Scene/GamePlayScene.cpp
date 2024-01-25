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
	collisionManager = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection_ = new ViewProjection();
	viewProjection_->Initialize();
	viewProjection_->SetEye({ -15.0f,0.0f,10.0f });
	viewProjection_->SetUp({ 0.0f,1.0f,0.0f });
	startCameraPos_ = viewProjection_->GetEye();
	startCamoreMovePos_ = { 15.0f,5.0f,30.0f };
	beforeTargetNum_ = viewProjection_->target_;
	normalTargetNum_ = viewProjection_->target_;
	normalUpNum_ = viewProjection_->up_;

	//天球
	sky_ = new SkyDome;
	sky_->SkyDomeInitialize();
	sky_->SetPosition({ 0.0f,0.0f,500.0f });

	//爆発
	explosion01_ = new Explosion;
	explosion01_->ExplosionInitialize(0);
	explosion02_ = new Explosion;
	explosion02_->ExplosionInitialize(1);
	explosion03_ = new Explosion;
	explosion03_->ExplosionInitialize(2);

	//player 
	player = new Player;
	player->PlayerInitialize();

	//bossEnemy
	bEnemy = new BossEnemy;
	bEnemy->BossEnemyInitialize();
	bEnemy->worldTransform_.UpdateMatrix();
	bEnemy->ColliderUpdate();

	//敵ファイル読み込み
	LoadEnemyPop();

	//パーティクル
	//ヒットテクスチャ
	p_Hit = Particle::LoadParticleTexture("effect01.png");
	pm_Hit = ParticleManager::Create();
	pm_Hit->SetParticleModel(p_Hit);
	pm_Hit->SetViewProjection(viewProjection_);
	//wEnemyテクスチャ
	p_WDmg = Particle::LoadParticleTexture("effect02.png");
	pm_WDmg = ParticleManager::Create();
	pm_WDmg->SetParticleModel(p_WDmg);
	pm_WDmg->SetViewProjection(viewProjection_);
	//bEnemyテクスチャ
	p_BDmg = Particle::LoadParticleTexture("effect03.png");
	pm_BDmg = ParticleManager::Create();
	pm_BDmg->SetParticleModel(p_BDmg);
	pm_BDmg->SetViewProjection(viewProjection_);
	//meteorテクスチャ
	p_Meteor = Particle::LoadParticleTexture("effect04.png");
	pm_Meteor = ParticleManager::Create();
	pm_Meteor->SetParticleModel(p_Meteor);
	pm_Meteor->SetViewProjection(viewProjection_);
	//基本爆発テクスチャ
	p_Ex = Particle::LoadParticleTexture("effect05.png");
	pm_Ex = ParticleManager::Create();
	pm_Ex->SetParticleModel(p_Ex);
	pm_Ex->SetViewProjection(viewProjection_);
	//自機爆発テクスチャ
	p_PEx = Particle::LoadParticleTexture("effect06.png");
	pm_PEx = ParticleManager::Create();
	pm_PEx->SetParticleModel(p_PEx);
	pm_PEx->SetViewProjection(viewProjection_);
	//自機爆発テクスチャ
	p_Smoke = Particle::LoadParticleTexture("effect07.png");
	pm_Smoke = ParticleManager::Create();
	pm_Smoke->SetParticleModel(p_Smoke);
	pm_Smoke->SetViewProjection(viewProjection_);

	//UIの初期化
	UIInitialize();

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

	//墜落量
	gameOverNum_ = 0;
	//最大
	gameOvernumMax_ = 60;
	//シェイク範囲
	shakeRange_ = 3.0f;
	//シェイク移動量
	shakeNum_ = 0.1f;
	//ボスの墜落スピード
	bossDownSpeed_ = 0.01f;
	//ボスalpha
	bossAlpha_ = 1.0f;
	bossAlphaMax_ = 1.0f;
	bossAlphaMin_ = 0.5f;
	//ボスalphaに代入する数
	bossAlphaNum_ = 0.05f;
	bossScaleNum_ = { 0.01f,0.01f,0.01f };

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
	bossShake_ = true;
	isBossAlpha_ = true;
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
	//自機登場演出
	PlayerInit();
	//UIの登場モーション
	UIInitMotion();
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
		if (isBossInitCamera_ == false && isClearScene_ == false && isBEnemyDeadScene_ == false)
		{
			MoveCamera();

			player->Update(worldTransform3DReticle_);
			player->ColliderUpdate();
		}
		else
		{
			player->BulletUpdate();
			bEnemy->BulletUpdate();
		}
		//UI更新
		UIUpdate();
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
	collisionManager->CheckAllCollisions();

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
					isRed_ = true;
					//赤フェードアウト
					if (redAlpha_ > redAlphaNumMin_)
					{
						redAlpha_ -= blackAlphaNum_;
						red_.SetAlpha(red_, redAlpha_);
					}
					red_.Update(red_, spriteCommon_);
					if (hitPlayerTimer_ % 2 != 1)
					{
						CameraShake(playerCameraShake_, playerCameraShake_);
					}
					else
					{
						viewProjection_->SetEye(cameraShakePos_);
					}
					//ダメージのイージング処理
					hpDamage_ = 0.0f + (1.0f * MathFunc::easeOutSine((float)hitPlayerTimer_ / (float)hitPlayerTimerMax_));
					//hpのスケールに残りhpを反映
					hpBar_.SetScale({ 24.0f,234.0f * ((((float)player->GetHp() + 1.0f) - hpDamage_) / (float)player->GetHpMax()) });
					hpBar_.SpriteTransferVertexBuffer(hpBar_, 11);
					//無敵時間
					hitPlayerTimer_++;
				}
				else
				{
					isRed_ = false;
					red_.SetAlpha(red_, redAlphaNumMax_);
					player->SetIsHit(false);
					player->SetIsInv(false);
					viewProjection_->SetEye(cameraShakePos_);
					hpDamage_ = 0.0f;
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
			pm_WDmg->Fire(p_WDmg, 40, deadPos, 12, false, { 1.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke, 30, deadPos, 5, false, { 5.0f, 0.0f });
		}
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
	{
		if (invEnemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = invEnemys->GetPosition();
			pm_Hit->Fire(p_Hit, 40, hitPos, 5, false, { 1.0f, 0.0f });
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
			pm_Meteor->Fire(p_Meteor, 40, deadPos, 12, false, { 1.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke, 30, deadPos, 5, false, { 5.0f, 0.0f });
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
			pm_Meteor->Fire(p_Meteor, 40, deadPos, 12, false, { 1.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 30, deadPos, 5, false, { 4.0f, 0.0f });
			pm_Ex->Fire(p_Ex, 20, deadPos, 1, true, { 18.0f, 0.0f });
			pm_Smoke->Fire(p_Smoke, 30, deadPos, 5, false, { 5.0f, 0.0f });
		}
	}

	//自機が残り体力1の時の煙
	if (player->GetHp() == 1)
	{
		Vector3 pPos = player->GetPosition();
		pm_Smoke->ChasePlayer(p_Smoke, 3, pPos,{ 1.0f, 0.0f });
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
				pm_Ex->Fire(p_Ex, 20, bossDeadPos_, 4, false, { 8.0f, 0.0f });
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

	sky_->Draw(viewProjection_);
	if (isPlayerDead_ == false)
	{
		//カメラシェイク中は点滅
		if (hitPlayerTimer_ % 2 != 1)
		{
			player->Draw(viewProjection_);
		}
		player->BulletDraw(viewProjection_);
	}

	//星屑
	for (auto& object : stardustObjects_) {
		object->Draw(viewProjection_, 1.0f, object->GetColor());
	}
	//隕石
	for (auto& object : meteorObjects_) {
		object->Draw(viewProjection_, object->GetAlpha(), object->GetColor());
	}

	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		if (meteors->GetIsDelete() == false)
		{
			meteors->Draw(viewProjection_, meteors->GetAlpha(), meteors->GetColor());
		}
	}
	for (std::unique_ptr<Item>& items : items_)
	{
		items->Draw(viewProjection_);
	}
	switch (gameNum_)
	{
	case FirstScene:
		//雑魚敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Draw(viewProjection_, 1.0f, wEnemys->GetColor());
		}
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Draw(viewProjection_);
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Draw(viewProjection_, 1.0f, enemys->GetColor());

			enemys->BulletDraw(viewProjection_);
		}
		break;

	case BossScene:
		//ボス敵
		if (bEnemy->GetIsDead() == false)
		{
			bEnemy->Draw(viewProjection_, bossAlpha_, bEnemy->GetColor());
			bEnemy->BulletDraw(viewProjection_);
		}
		break;
	}

	if (isBEnemyDeadScene_ == true)
	{
		explosion01_->ExplosionDraw(viewProjection_);
		explosion02_->ExplosionDraw(viewProjection_);
	}

	if (isPlayerDead_ == true && explosion03_->GetIsFinish() == false)
	{
		explosion03_->ExplosionDraw(viewProjection_);
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

	//描画前処理
	Sprite::PreDraw(dxCommon_->GetCommandList(), spriteCommon_);

	UIDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void GamePlayScene::Finalize()
{
	for (Stardust*& object : stardustObjects_) {
		delete(object);
	}
	for (Meteor*& object : meteorObjects_) {
		delete(object);
	}

	delete sky_;
	delete player;
	delete enemy;
	delete wEnemy;
	delete bEnemy;
	delete invincibleEnemy;
	delete p_Hit;
	delete pm_Hit;
	delete p_WDmg;
	delete pm_WDmg;
	delete p_BDmg;
	delete pm_BDmg;
	delete p_Meteor;
	delete pm_Meteor;
	delete p_Ex;
	delete pm_Ex;
	delete p_PEx;
	delete pm_PEx;
	delete p_Smoke;
	delete pm_Smoke;
	delete explosion01_;
	delete explosion02_;
	delete explosion03_;

	//カメラ解放
	delete viewProjection_;
}

void GamePlayScene::GameReset()
{
	viewProjection_->Initialize();
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
		//黒フェードアウト
		if (blackAlpha_ > blackAlphaNumMin_)
		{
			blackAlpha_ -= blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
		black_.Update(black_, spriteCommon_);
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
		pm_Smoke->PlayerExBefore(p_Smoke, 20, player->GetPosition(), 1, { 2.0f, 0.0f });
		pm_Ex->PlayerExBefore(p_Ex, 20, player->GetPosition(), 2, { 2.0f, 0.0f });
		//自機を動かす
		player->worldTransform_.rotation_.z += 8.0f;
		if (player->worldTransform_.rotation_.z >= 720.0f)
		{
			isPlayerDead_ = true;
			pm_PEx->PlayerExAfter(p_PEx, 200, player->GetPosition(), 12, { 1.0f, 0.0f });
		}
	}
}

void GamePlayScene::BossInit()
{
	if (isBossInitCamera_ == true)
	{
		isUp_ = false;
		isDown_ = false;
		isRight_ = false;
		isLeft_ = false;

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
			isInvicibleReticle_ = false;
			//寄ったら黒フェード
			if (blackAlpha_ < blackAlphaNumMax_)
			{
				blackAlpha_ += blackAlphaNum_;
				black_.SetAlpha(black_, blackAlpha_);
			}
			else
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
		if (blackAlpha_ > blackAlphaNumMin_)
		{
			blackAlpha_ -= blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
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
			isInvicibleReticle_ = true;
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
	UIOutMotion();
	explosion01_->EnemyExplosionUpdate();
	explosion02_->EnemyExplosionUpdate();
	//墜落
	if (bEnemy->worldTransform_.position_.x > 0.0f)
	{
		bEnemy->worldTransform_.position_.y -= bossDownSpeed_;
	}

	bEnemy->worldTransform_.scale_ -= bossScaleNum_;

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
		if (bossAlpha_ <= bossAlphaMax_)
		{
			bossAlpha_ += bossAlphaNum_;
		}
		else
		{
			isBossAlpha_ = false;
		}
	}
	else
	{
		if (bossAlpha_ >= bossAlphaMin_)
		{
			bossAlpha_ -= bossAlphaNum_;
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
		if (isClearCameraWork_ == false)
		{
			//カメラワーク
			ToClearCameraWork();
		}
		else
		{
			//黒フェードイン
			if (blackAlpha_ < blackAlphaNumMax_)
			{
				blackAlpha_ += blackAlphaNum_;
				black_.SetAlpha(black_, blackAlpha_);
			}
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
		UIOutMotion();
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
				if (blackAlpha_ < blackAlphaNumMax_)
				{
					blackAlpha_ += blackAlphaNum_;
					black_.SetAlpha(black_, blackAlpha_);
				}
				gameOverNum_++;
				//gameoverシーンへ
				if (gameOverNum_ >= gameOvernumMax_)
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
		isUp_ = true;
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, 0.1f, 0));
	}
	else
	{
		isUp_ = false;
		if (viewProjection_->target_.y >= normalTargetNum_.y)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -0.1f, 0));
		}
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		isRight_ = true;
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0.1f, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(-0.0025f, 0, 0));
	}
	else
	{
		isRight_ = false;
		if (viewProjection_->target_.x >= normalTargetNum_.x)
		{
			viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-0.1f, 0, 0));
		}
		if (viewProjection_->up_.x <= normalUpNum_.x)
		{
			viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
		}
	}
	arrowRight_.SetColor(arrowRight_, { uiColorPush_,uiColorPush_,uiColorPush_,1.0f });
	if (input_->PushKey(DIK_LEFT))
	{
		isLeft_ = true;
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(-0.1f, 0, 0));
		viewProjection_->SetUp(viewProjection_->up_ + Vector3(0.0025f, 0, 0));
	}
	else
	{
		isLeft_ = false;
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
		isDown_ = true;
		viewProjection_->SetTarget(viewProjection_->target_ + Vector3(0, -0.1f, 0));
	}
	else
	{
		isDown_ = false;
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
	backGroundStardust_ = LevelLoader::LoadFile("stardust");
	backGroundMeteor_ = LevelLoader::LoadFile("meteor");

	// レベルデータからオブジェクトを生成、配置
	//星屑
	for (auto& objectData : backGroundStardust_->objects) {

		// モデルを指定して3Dオブジェクトを生成
		backStardust = new Stardust;
		backStardust->StardustInitialize();
		//モデル読み込み
		backModelStardust = Model::LoadFromOBJ("stardust");
		stardustModels_.insert(std::make_pair("stardust", backModelStardust));
		backStardust->SetModel(backModelStardust);
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		backStardust->SetPosition(pos);
		//初期Y座標を保存しておく
		backStardust->SetBeforeY(pos.y);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		backStardust->SetRotation(rot);

		backStardust->SetColor();
		backStardust->SetSize();

		// 配列に登録
		stardustObjects_.push_back(backStardust);
	}
	//隕石
	for (auto& objectData : backGroundMeteor_->objects) {

		// モデルを指定して3Dオブジェクトを生成
		backMeteor = new Meteor;
		backMeteor->MeteorInitialize();
		//モデル読み込み
		backModelMeteor = Model::LoadFromOBJ("bigMeteor");
		meteorModels_.insert(std::make_pair("meteor", backModelMeteor));
		backMeteor->SetModel(backModelMeteor);
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		backMeteor->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		backMeteor->SetRotation(rot);

		// 大きさ
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		backMeteor->SetScale(scale);

		backMeteor->SetIsBack(true);
		backMeteor->SetAlpha(1.0f);
		backMeteor->SetSpeed(0.1f, 1.0f);

		// 配列に登録
		meteorObjects_.push_back(backMeteor);
	}
}

void GamePlayScene::UIInitialize()
{
	//UI入退場の変数
	UIInitPos_ = 0.0f;
	UIInitRange_ = 100.0f;
	UIInitTime_ = 0.0f;
	UIOutPos_ = 0.0f;
	UIOutRange_ = 100.0f;
	UIOutTime_ = 0.0f;
	UIMovePos_ = 0.0f;
	UIMoveRange_ = 5.0f;
	UIMoveTime_ = 0.0f;

	//ダメージ
	hpDamage_ = 0.0f;
	//画面端との距離
	UiRange_ = 30.0f;
	//押されていないときの色
	uiColorNormal_ = 0.5f;
	//押されているときの色
	uiColorPush_ = 3.0f;
	//現在の色
	for (int i = 0; i < 4; i++)
	{
		uiColor_[i] = uiColorNormal_;
	}

	//色の変動値
	uiColorNum_ = 0.2f;

	isUIInit_ = false;
	isUIOut_ = false;
	isNeutral_ = true;
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;

	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice());

	//上
	arrowUp_.Initialize(dxCommon_->GetDevice(), 0, Vector2(0.5f, 0.5f), false, false);
	arrowUp_.SetScale({ 201,50 });
	arrowUp_.SetPosition({ 640,-50,0 });
	arrowUp_.SpriteTransferVertexBuffer(arrowUp_, 0);
	arrowUp_.Update(arrowUp_, spriteCommon_);
	arrowUp_.LoadTexture(spriteCommon_, 0, L"Resources/2d/up.png", dxCommon_->GetDevice());

	//下
	arrowDown_.Initialize(dxCommon_->GetDevice(), 1, Vector2(0.5f, 0.5f), false, false);
	arrowDown_.SetScale({ 201,50 });
	arrowDown_.SetPosition({ 640,770,0 });
	arrowDown_.SpriteTransferVertexBuffer(arrowDown_, 1);
	arrowDown_.Update(arrowDown_, spriteCommon_);
	arrowDown_.LoadTexture(spriteCommon_, 1, L"Resources/2d/down.png", dxCommon_->GetDevice());

	//右
	arrowRight_.Initialize(dxCommon_->GetDevice(), 2, Vector2(0.5f, 0.5f), false, false);
	arrowRight_.SetScale({ 50,201 });
	arrowRight_.SetPosition({ 1330,360,0 });
	arrowRight_.SpriteTransferVertexBuffer(arrowRight_, 2);
	arrowRight_.Update(arrowRight_, spriteCommon_);
	arrowRight_.LoadTexture(spriteCommon_, 2, L"Resources/2d/right.png", dxCommon_->GetDevice());

	//左
	arrowLeft_.Initialize(dxCommon_->GetDevice(), 3, Vector2(0.5f, 0.5f), false, false);
	arrowLeft_.SetScale({ 50,201 });
	arrowLeft_.SetPosition({ -50,360,0 });
	arrowLeft_.SpriteTransferVertexBuffer(arrowLeft_, 3);
	arrowLeft_.Update(arrowLeft_, spriteCommon_);
	arrowLeft_.LoadTexture(spriteCommon_, 3, L"Resources/2d/left.png", dxCommon_->GetDevice());

	//黒
	blackAlpha_ = 1.0f;
	blackAlphaNum_ = 0.02f;
	blackAlphaNumMax_ = 1.0f;
	blackAlphaNumMin_ = 0.0f;
	black_.Initialize(dxCommon_->GetDevice(), 4, Vector2(0.0f, 0.0f), false, false);
	black_.SetScale(Vector2(1280 * 1, 720 * 1));
	black_.SetPosition({ 0,0,0 });
	black_.SpriteTransferVertexBuffer(black_, 4);
	black_.SetAlpha(black_, blackAlpha_);
	black_.Update(black_, spriteCommon_);
	black_.LoadTexture(spriteCommon_, 4, L"Resources/2d/black.png", dxCommon_->GetDevice());

	//赤
	redAlpha_ = 0.5f;
	redAlphaNum_ = 0.02f;
	redAlphaNumMax_ = 0.5f;
	redAlphaNumMin_ = 0.0f;
	isRed_ = false;
	red_.Initialize(dxCommon_->GetDevice(), 5, Vector2(0.0f, 0.0f), false, false);
	red_.SetScale(Vector2(1280 * 1, 720 * 1));
	red_.SetPosition({ 0,0,0 });
	red_.SpriteTransferVertexBuffer(red_, 5);
	red_.SetAlpha(red_, redAlpha_);
	red_.Update(red_, spriteCommon_);
	red_.LoadTexture(spriteCommon_, 5, L"Resources/2d/red.png", dxCommon_->GetDevice());

	//レティクル
	reticle_.Initialize(dxCommon_->GetDevice(), 6, Vector2(0.5f, 0.5f), false, false);
	reticle_.SetScale({ 100,100 });
	reticle_.SetPosition({ 0,0,0 });
	reticle_.SetColor(reticle_, { uiColorPush_,uiColorPush_ ,uiColorPush_ ,1.0f });
	reticle_.SetAlpha(reticle_, reticleAlpha_);
	reticle_.SpriteTransferVertexBuffer(reticle_, 6);
	reticle_.Update(reticle_, spriteCommon_);
	reticle_.LoadTexture(spriteCommon_, 6, L"Resources/2d/reticle.png", dxCommon_->GetDevice());

	//hp関係
	hpBar_.Initialize(dxCommon_->GetDevice(), 7, Vector2(0.5f, 1.0f), false, false);
	hpBar_.SetScale({ 24,234 });
	hpBar_.SpriteTransferVertexBuffer(hpBar_, 7);
	hpBar_.Update(hpBar_, spriteCommon_);
	hpBar_.LoadTexture(spriteCommon_, 7, L"Resources/2d/hpBar.png", dxCommon_->GetDevice());
	//枠
	hpFrame_.Initialize(dxCommon_->GetDevice(), 8, Vector2(0.5f, 1.0f), false, false);
	hpFrame_.SetScale({ 30,240 });
	hpFrame_.SpriteTransferVertexBuffer(hpFrame_, 8);
	hpFrame_.Update(hpFrame_, spriteCommon_);
	hpFrame_.LoadTexture(spriteCommon_, 8, L"Resources/2d/hpFrame.png", dxCommon_->GetDevice());

	//緑
	greenAlpha_ = 0.1f;
	green_.Initialize(dxCommon_->GetDevice(), 9, Vector2(0.0f, 0.0f), false, false);
	green_.SetScale(Vector2(1280 * 1, 720 * 1));
	green_.SetPosition({ 0,0,0 });
	green_.SpriteTransferVertexBuffer(green_, 9);
	green_.SetAlpha(green_, greenAlpha_);
	green_.Update(green_, spriteCommon_);
	green_.LoadTexture(spriteCommon_, 9, L"Resources/2d/green.png", dxCommon_->GetDevice());

	//スコア
	score_.Initialize(dxCommon_->GetDevice(), 10, Vector2(0.0f, 0.0f), false, false);
	score_.SetScale({ 140,60 });
	score_.SetPosition({ 50,0 - UIInitRange_,0 });
	score_.SetColor(score_, { uiColorPush_,uiColorPush_ ,uiColorPush_ ,1.0f });
	score_.SpriteTransferVertexBuffer(score_, 10);
	score_.Update(score_, spriteCommon_);
	score_.LoadTexture(spriteCommon_, 10, L"Resources/2d/score.png", dxCommon_->GetDevice());

	//レティクル関係変数初期化
	reticleWorldPos_ = { 0.0f,0.0f,0.0f };
	matViewport_.identity();
	matViewprojectionViewport_.identity();

	//レティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	distancePlayerToReticle_ = 50.0f;
	reticleAlpha_ = 0.0f;
	reticleAlphaNum_ = 0.1f;
	reticleAlphaNumMax_ = 1.0f;
	reticleAlphaNumMin_ = 0.0f;
	isInvicibleReticle_ = true;
}

void GamePlayScene::UIInitMotion()
{
	if (isUIInit_ == false && isPlayerInit_ == true)
	{
		isInvicibleReticle_ = true;
		UIInitPos_ = UIInitRange_ * MathFunc::easeOutSine(UIInitTime_ / 30.0f);
		UIInitTime_++;
		arrowUp_.SetPosition({ 640,UiRange_ - UIInitRange_ + UIInitPos_,0 });
		arrowDown_.SetPosition({ 640,720 - UiRange_ + UIInitRange_ - UIInitPos_,0 });
		arrowRight_.SetPosition({ 1280 - UiRange_ + UIInitRange_ - UIInitPos_,360,0 });
		arrowLeft_.SetPosition({ UiRange_ - UIInitRange_ + UIInitPos_,360,0 });
		hpBar_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,697,0 });
		hpFrame_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,700,0 });
		score_.SetPosition({ 30, 10 - UIInitRange_ + UIInitPos_, 0 });
	}
	if (UIInitPos_ >= UIInitRange_)
	{
		isUIInit_ = true;
	}
}

void GamePlayScene::UIOutMotion()
{
	isNeutral_ = false;
	isInvicibleReticle_ = false;
	if (isUIOut_ == false)
	{
		if (UIOutPos_ < UIOutRange_)
		{
			UIOutPos_ = UIOutRange_ * MathFunc::easeOutSine(UIOutTime_ / 30.0f);
			UIOutTime_++;
			arrowUp_.SetPosition({ 640.0f,UiRange_ - UIOutPos_,0.0f });
			arrowDown_.SetPosition({ 640.0f,720.0f - UiRange_ + UIOutPos_,0.0f });
			arrowRight_.SetPosition({ 1280 - UiRange_ + UIOutPos_,360.0f,0.0f });
			arrowLeft_.SetPosition({ UiRange_ - UIOutPos_,360.0f,0.0f });
			hpBar_.SetPosition({ 0 - UIOutPos_,697,0 });
			hpFrame_.SetPosition({ 0 - UIOutPos_,700,0 });
			score_.SetPosition({ 30.0f,10.0f - UIOutPos_,0.0f });
		}
		else
		{
			UIOutPos_ = UIOutRange_;
			isUIOut_ = true;
		}
	}
}

void GamePlayScene::UIUpdate()
{
	//登場後モーション
	UIMove();
	//色変化
	UIColorChange();

	//更新
	arrowUp_.Update(arrowUp_, spriteCommon_);
	arrowDown_.Update(arrowDown_, spriteCommon_);
	arrowRight_.Update(arrowRight_, spriteCommon_);
	arrowLeft_.Update(arrowLeft_, spriteCommon_);
	black_.Update(black_, spriteCommon_);
	red_.Update(red_, spriteCommon_);
	green_.Update(green_, spriteCommon_);
	hpBar_.Update(hpBar_, spriteCommon_);
	hpFrame_.Update(hpFrame_, spriteCommon_);
	score_.Update(score_, spriteCommon_);

	ReticleUpdate();
}

void GamePlayScene::UIDraw()
{
	//移動時のUI切り替え
	arrowUp_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowDown_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowRight_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowLeft_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	//画面系
	//green_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	black_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	if (isRed_ == true)
	{
		red_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	}
	reticle_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	hpFrame_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	hpBar_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	score_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
}

void GamePlayScene::UIMove()
{
	if (isUIInit_ == true && isNeutral_ == true)
	{
		if (isMove_ == true)
		{
			UIMovePos_ = UIMoveRange_ * MathFunc::easeOutSine(UIMoveTime_ / 30.0f);
			UIMoveTime_++;
			if (UIMoveTime_ >= 60.0f)
			{
				isMove_ = false;
				UIMoveTime_ = 0.0f;
			}
		}
		else
		{
			UIMovePos_ = UIMoveRange_ * -MathFunc::easeOutSine(UIMoveTime_ / 30.0f);
			UIMoveTime_++;
			if (UIMoveTime_ >= 60.0f)
			{
				isMove_ = true;
				UIMoveTime_ = 0.0f;
			}
		}
		arrowUp_.SetPosition({ 640,UiRange_ - UIMovePos_,0 });
		arrowDown_.SetPosition({ 640,720 - UiRange_ + UIMovePos_,0 });
		arrowRight_.SetPosition({ 1280 - UiRange_ + UIMovePos_,360,0 });
		arrowLeft_.SetPosition({ UiRange_ - UIMovePos_ ,360,0 });
	}
}

void GamePlayScene::UIColorChange()
{
	//キーを押しているかで色を変化させる
	if (isUp_ == true)
	{
		if (uiColor_[UP] < uiColorPush_)
		{
			uiColor_[UP] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[UP] > uiColorNormal_)
		{
			uiColor_[UP] -= uiColorNum_;
		}
	}
	if (isDown_ == true)
	{
		if (uiColor_[DOWN] < uiColorPush_)
		{
			uiColor_[DOWN] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[DOWN] > uiColorNormal_)
		{
			uiColor_[DOWN] -= uiColorNum_;
		}
	}
	if (isRight_ == true)
	{
		if (uiColor_[RIGHT] < uiColorPush_)
		{
			uiColor_[RIGHT] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[RIGHT] > uiColorNormal_)
		{
			uiColor_[RIGHT] -= uiColorNum_;
		}
	}
	if (isLeft_ == true)
	{
		if (uiColor_[LEFT] < uiColorPush_)
		{
			uiColor_[LEFT] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[LEFT] > uiColorNormal_)
		{
			uiColor_[LEFT] -= uiColorNum_;
		}
	}
	//色をセット
	arrowUp_.SetColor(arrowUp_, { uiColor_[UP],uiColor_[UP] ,uiColor_[UP] ,1.0f });
	arrowDown_.SetColor(arrowDown_, { uiColor_[DOWN],uiColor_[DOWN] ,uiColor_[DOWN] ,1.0f });
	arrowRight_.SetColor(arrowRight_, { uiColor_[RIGHT],uiColor_[RIGHT] ,uiColor_[RIGHT] ,1.0f });
	arrowLeft_.SetColor(arrowLeft_, { uiColor_[LEFT],uiColor_[LEFT] ,uiColor_[LEFT] ,1.0f });
}

void GamePlayScene::ReticleUpdate()
{
	if (isInvicibleReticle_ == true)
	{
		if (reticleAlpha_ < reticleAlphaNumMax_)
		{
			reticleAlpha_ += reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	else
	{
		if (reticleAlpha_ > reticleAlphaNumMin_)
		{
			reticleAlpha_ -= reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offSet = { 0,0,1.0f };
		//自機のワールド行列の回転を反映
		offSet = MatVector(offSet, player->worldTransform_.matWorld_);
		//長さを整える
		offSet = offSet.normalize() * distancePlayerToReticle_;
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.position_ = Vector3::AddVector3(player->GetWorldPosition(), offSet);
		worldTransform3DReticle_.UpdateMatrix();
	}
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// 3Dレティクルのワールド行列から,ワールド座標を取得
		reticleWorldPos_.x = worldTransform3DReticle_.matWorld_.m[3][0];
		reticleWorldPos_.y = worldTransform3DReticle_.matWorld_.m[3][1];
		reticleWorldPos_.z = worldTransform3DReticle_.matWorld_.m[3][2];

		//ビューポート行列
		matViewport_ = SetViewport(Vector3(0, 0, 0));

		//ビュー行列とプロジェクション行列,ビューポート行列を合成する
		matViewprojectionViewport_ =
			viewProjection_->matView_ * viewProjection_->matProjection_ * matViewport_;
		//ワールド→スクリーン座標変換
		reticleWorldPos_ = Transform(reticleWorldPos_, matViewprojectionViewport_);
		//座標設定
		reticle_.SetPosition(Vector3(reticleWorldPos_.x, reticleWorldPos_.y, 0.0f));
	}
	reticle_.Update(reticle_, spriteCommon_);
}

Vector3 GamePlayScene::MatVector(Vector3 v, Matrix4 mat)
{
	Vector3 pos;
	pos.x = -mat.m[0][0] * v.x + -mat.m[0][1] * v.y + -mat.m[0][2] * v.z + mat.m[0][3] * 1;
	pos.y = -mat.m[1][0] * v.x + -mat.m[1][1] * v.y + -mat.m[1][2] * v.z + mat.m[1][3] * 1;
	pos.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * 1;

	return pos;
}

Matrix4 GamePlayScene::SetViewport(const Vector3& v)
{
	//単位行列の設定
	Matrix4 matViewport = Matrix4::identity();
	matViewport.m[0][0] = 1280.0f / 2.0f;
	matViewport.m[1][1] = -720.0f / 2.0f;
	matViewport.m[3][0] = (1280.0f / 2.0f) + v.x;
	matViewport.m[3][1] = (720.0f / 2.0f) + v.y;
	return matViewport;
}

Vector3 GamePlayScene::Transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}