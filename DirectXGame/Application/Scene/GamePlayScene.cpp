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
	xmViewProjection = new XMViewProjection();
	viewProjection_->Initialize();
	viewProjection_->SetEye({ -15.0f,0.0f,10.0f });
	viewProjection_->SetUp({ 0.0f,1.0f,0.0f });
	startCameraPos_ = viewProjection_->GetEye();
	beforeTargetNum_ = viewProjection_->target_;
	normalTargetNum_ = viewProjection_->target_;
	normalUpNum_ = viewProjection_->up_;

	//天球
	sky_ = new SkyDome;
	sky_->SkyDomeInitialize();
	sky_->SetPosition({ 0.0f,0.0f,500.0f });

	//黒
	black_ = new Black;
	black_->BlackInitialize();
	black_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z - 2.0f));

	//黒
	red_ = new Red;
	red_->RedInitialize();
	red_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z + 1.0f));

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

	//敵ファイル読み込み
	LoadEnemyPop();

	//パーティクル
	//自機の弾テクスチャ
	p_dmg = Particle::LoadParticleTexture("effect01.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);
	//wEnemyテクスチャ
	p_wDmg = Particle::LoadParticleTexture("effect02.png");
	pm_eDmg = ParticleManager::Create();
	pm_eDmg->SetParticleModel(p_wDmg);
	pm_eDmg->SetXMViewProjection(xmViewProjection);
	//bEnemyテクスチャ
	p_bDmg = Particle::LoadParticleTexture("effect03.png");
	pm_bDmg = ParticleManager::Create();
	pm_bDmg->SetParticleModel(p_bDmg);
	pm_bDmg->SetXMViewProjection(xmViewProjection);
	//meteorテクスチャ
	p_meteor = Particle::LoadParticleTexture("effect04.png");
	pm_meteor = ParticleManager::Create();
	pm_meteor->SetParticleModel(p_meteor);
	pm_meteor->SetXMViewProjection(xmViewProjection);

	UIInitialize();

	LoadLevelData();

	//メンバ変数の初期化
	cameraPos_ = CameraPos::Back;
	cameraShakePos_ = { 0.0f,0.0f,0.0f };
	deadCameraPos_ = { 0.0f,0.0f,0.0f };
	bossInitCameraPos_ = { 0.0f,0.0f,0.0f };

	//基本target
	changeTargetNum_ = { 0.0f,0.0f,0.0f };

	//ゲーム中のシーン番号
	gameNum_ = GameNum::FirstScene;
	//カメラワーク番号
	bossInitNum_ = BossInitNum::Up;

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
	//ボスalphaに代入する数
	bossAlphaNum_ = 0.05f;

	clearCameraNum_ = 0;

	//フラグ
	isWait_ = false;
	isClearScene_ = false;
	isBossEffect_ = false;
	isBEnemyDeadScene_ = false;
	bossShake_ = true;
	isBossAlpha_ = true;
	isBossInitCamera_ = false;
	isStart_ = false;
	isChangeCameraDir_ = false;
	isPassPoint_ = false;
	isDeadCameraShake_ = false;

	//タイマー
	delayTimer_ = 0.0f;
	bossAppTimer_ = 0.0f;
	waitTimer_ = 0;
	clearTimer_ = 0.0f;
	bossInitTimer_ = 0;
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
	sky_->SkyDomeUpdate();
	//自機登場演出
	PlayerInit();
	//UIの登場モーション
	UIInitMotion();
	//"CHANGE"を読み取ったらカメラ切り替え
	ChangeCameraDirection();
	for (auto& object : stardustObjects_)
	{
		object->StardustUpdate();
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
	}

	switch (gameNum_)
	{
	case FirstScene://雑魚敵戦専用の処理

		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->ColliderUpdate();
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
		{
			invEnemys->Update(player->GetPosition());
			invEnemys->ColliderUpdate();
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Update(player->GetPosition());
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
					if (bEnemy->GetDeathTimer() % 2 != 1)
					{
						CameraShake(0.3f, 0.3f);
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
		if (isPlayerDead_ == false)
		{
			black_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z + 2.0f));
		}
		if (isBossInitCamera_ == false && isClearScene_ == false && isBEnemyDeadScene_ == false && isChangeCameraDir_ == false)
		{
			MoveCamera();

			player->Update();
			player->ColliderUpdate();

			//パーティクル更新
			pm_dmg->Update();
			pm_eDmg->Update();
			pm_bDmg->Update();
			//UI更新
			UIUpdate();
		}
		else
		{
			//UI更新
			UIUpdate();
			//パーティクル更新
			pm_dmg->Update();
			pm_eDmg->Update();
			pm_bDmg->Update();
			player->BulletUpdate();
			bEnemy->BulletUpdate();
		}
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
				//無敵時間
				hitPlayerTimer_++;
				if (hitPlayerTimer_ < hitPlayerTimerMax_)
				{
					red_->SetIsRed(true);
					if (hitPlayerTimer_ % 2 != 1)
					{
						CameraShake(0.2f, 0.2f);
					}
					else
					{
						viewProjection_->SetEye(cameraShakePos_);
					}
				}
				else
				{
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
			pm_eDmg->Fire(p_wDmg, 20, { deadPos.x,deadPos.y,deadPos.z }, 0, 5, { 8.0f, 0.0f });
		}
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
	{
		if (invEnemys->GetIsHit() == true)
		{
			Vector3 hitPos{};
			hitPos = invEnemys->GetPosition();
			pm_dmg->Fire(p_dmg, 10, { hitPos.x,hitPos.y,hitPos.z }, 0, 5, { 4.0f, 0.0f });
		}
	}

	//敵の被ダメージ処理
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		if (enemys->GetIsDead() == true)
		{
			Vector3 deadPos{};
			deadPos = enemys->GetPosition();
			pm_dmg->Fire(p_dmg, 20, { deadPos.x,deadPos.y,deadPos.z }, 0, 12, { 4.0f, 0.0f });
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
			pm_dmg->Fire(p_dmg, 20, { deadPos.x,deadPos.y,deadPos.z }, 0, 5, { 8.0f, 0.0f });
		}
	}

	if (isDeadCameraShake_ == true)
	{
		hitEnemyTimer_++;
		if (hitEnemyTimer_ < 16)
		{
			if (hitEnemyTimer_ % 2 != 1)
			{
				CameraShake(0.3f, 0.3f);
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
				pm_bDmg->Fire(p_bDmg, 50, { bossDeadPos_.x,bossDeadPos_.y,bossDeadPos_.z }, 0, 4, { 4.0f, 0.0f });
			}
		}
	}
	ToGameOverScene();

	black_->BlackUpdate();
	red_->RedUpdate();

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
		object->Draw(viewProjection_,1.0f,object->GetColor());
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
			wEnemys->Draw(viewProjection_);
		}
		for (std::unique_ptr<InvEnemy>& invEnemys : invEnemys_)
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

	pm_dmg->Draw();
	pm_eDmg->Draw();
	pm_bDmg->Draw();
	pm_meteor->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

	UIDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	red_->RedDraw(viewProjection_);
	black_->BlackDraw(viewProjection_);

	Object3d::PostDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void GamePlayScene::Finalize()
{
	for (Stardust*& object : stardustObjects_) {
		delete(object);
	}

	delete sky_;
	delete player;
	delete enemy;
	delete wEnemy;
	delete bEnemy;
	delete invEnemy;
	delete p_dmg;
	delete pm_dmg;
	delete explosion01_;
	delete explosion02_;
	delete black_;
	delete red_;

	//カメラ解放
	delete viewProjection_;
	delete xmViewProjection;
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
			newInvEnemy->SetBeforeY(position.y);
			position.y += 20.0f;
			// 座標データに追加
			newInvEnemy->SetPosition(position);
			newInvEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newInvEnemy->worldTransform_.UpdateMatrix();
			//登録
			invEnemys_.push_back(std::move(newInvEnemy));
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
		//"CHANGE"を読み取ってカメラ向き変更
		else if (key == "CHANGE")
		{
			isChangeCameraDir_ = true;
			cameraDirChangePos_ = { -viewProjection_->eye_.x,viewProjection_->eye_.y,-viewProjection_->eye_.z };
			beforeMoveCameraPos_ = viewProjection_->GetEye();
			changeTargetNum_ = viewProjection_->target_;
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
	invEnemys_.remove_if([](std::unique_ptr <InvEnemy>& invEnemy)
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
		player->IntitMotion();
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
			viewProjection_->eye_.x = startCameraPos_.x + 15.0f * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.y = startCameraPos_.y + 5.0f * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->eye_.z = startCameraPos_.z + 30.0f * -MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			viewProjection_->target_.z = changeTargetNum_.z + 50.0f * MathFunc::easeOutSine(startTimer_ / startTimerMax_);
			startTimer_++;
			if (startTimer_ > startTimerMax_)
			{
				viewProjection_->SetEye({ 0.0f, 5.0f, -20.0f });
				viewProjection_->SetTarget({ 0.0f, -2.0f, 50.0f });
				beforeEyeNum_ = { 0.0f,0.0f,0.0f };
				beforeTargetNum_ = { 0.0f,0.0f,0.0f };
				beforeUpNum_ = { 0.0f,0.0f,0.0f };
				normalEyeNum_ = viewProjection_->eye_;
				normalTargetNum_ = viewProjection_->target_;
				normalUpNum_ = viewProjection_->up_;
				black_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z + 2.0f));
				red_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z + 1.0f));
				isStart_ = true;
			}
		}
	}
}

void GamePlayScene::PlayerDead()
{
	//自機を動かす
	player->worldTransform_.rotation_.z += 8.0f;
	if (player->worldTransform_.rotation_.z >= 720.0f)
	{
		isPlayerDead_ = true;
		explosion03_->PlayerExplosionUpdate(player->GetPosition());
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
		if (viewProjection_->eye_.z <= 10.0f)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() + Vector3(0, 0, 10) * MathFunc::easeInSine(bossAppTimer_ / 180.0f));
		}
		else
		{
			//寄ったら黒フェード
			black_->SetIsFinish(true);
		}
		bossAppTimer_++;

		//黒フェードが完了したら登場カメラワークへ
		if (black_->GetAlpha() >= 1.0f)
		{
			bossInitNum_ = InitCameraWork;
			bossAppTimer_ = 0;
			viewProjection_->up_ = { 0.0f,1.0f,0.0f };
		}
		break;
	case InitCameraWork:
		black_->SetIsStart(true);
		viewProjection_->SetTarget(bEnemy->GetPosition());
		bEnemy->InitMotion();

		if (bEnemy->GetIsInit() == true)
		{
			//登場カメラシェイク時間
			bossInitTimer_++;
			if (bossInitTimer_ < 24)
			{
				if (bossInitTimer_ % 2 != 1)
				{
					CameraShake(1.5f, 1.5f);
				}
				else
				{
					viewProjection_->SetEye(bossInitCameraPos_);
				}
			}
			else
			{
				bossInitTimer_ = 0;
				black_->SetIsStart(false);
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
		if (viewProjection_->eye_.z > -20.0f)
		{
			viewProjection_->SetEye(viewProjection_->GetEye() - Vector3(0, 0, 1.0f));
		}
		else
		{
			viewProjection_->eye_.z = -20.0f;
			isBossInitCamera_ = false;
			bossInitNum_ = None;
		}
		break;
	}
}
void GamePlayScene::BossDead()
{
	UIOutMotion();
	explosion01_->EnemyExplosionUpdate(bossDeadPos_);
	explosion02_->EnemyExplosionUpdate(bossDeadPos_);
	//墜落
	bEnemy->worldTransform_.position_.y -= bossDownSpeed_;
	bEnemy->worldTransform_.scale_ -= Vector3(0.005f, 0.005f, 0.005f);
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
			bossAlpha_ += bossAlphaNum_;
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
			ToClearCameraWork();
		}
		else
		{
			black_->SetIsFinish(true);
			//自機を動かす
			player->worldTransform_.rotation_.z++;
			player->worldTransform_.position_.x -= 0.05f;
			player->worldTransform_.position_.z++;
			viewProjection_->eye_.z -= 0.5f;
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
		UIOutMotion();
		ToGameOverCameraWork();
		PlayerDead();
		player->worldTransform_.UpdateMatrix();
		if (isGameOver_ == true)
		{
			CameraShake(0.1f, 0.0f);
			if (explosion03_->GetIsFinish() == true)
			{
				viewProjection_->eye_ = normalEyeNum_;
				black_->SetIsFinish(true);
				gameOverNum_++;
				// ワールドトランスフォームの行列更新と転送
				if (gameOverNum_ >= 60)
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
			viewProjection_->eye_.x = deadCameraPos_.x + 20.0f * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			viewProjection_->eye_.z = deadCameraPos_.z + 35.0f * MathFunc::easeOutSine(playerDeadTimer_ / playerDeadTimerMax_);
			playerDeadTimer_++;
		}
		else
		{
			normalEyeNum_ = viewProjection_->eye_;
			black_->SetRotation({ 0.0f,-90.0f,0.0f });
			black_->SetPosition(Vector3(viewProjection_->eye_.x - 1.0f, viewProjection_->eye_.y, viewProjection_->eye_.z - 2.0f));
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

void GamePlayScene::ChangeCameraDirection()
{
	if (isChangeCameraDir_ == true)
	{
		if (cameraPos_ == CameraPos::Back)
		{
			cameraMovePoint_ = { -20.0f,5.0f,0.0f };
			cameraMoveZ_ = 20.0f;
			beforeTargetNum_.z = 50.0f;
			moveTargetZ_ = beforeTargetNum_.z * -2.0f;
			player->SetIsChangeDir(true);
		}
		else if (cameraPos_ == CameraPos::Front)
		{
			cameraMovePoint_ = { 20.0f,5.0f,0.0f };
			cameraMoveZ_ = -20.0f;
			beforeTargetNum_.z = -50.0f;
			moveTargetZ_ = beforeTargetNum_.z * -2.0f;
			player->SetIsChangeDir(false);
		}
		CameraMovePoint();
	}
}

void GamePlayScene::CameraMovePoint()
{
	//ターゲット移動処理
	if (targetMoveTimer_ < targetMoveTimerMax_)
	{
		viewProjection_->target_.z = changeTargetNum_.z + moveTargetZ_ * MathFunc::easeOutSine(targetMoveTimer_ / targetMoveTimerMax_);
		targetMoveTimer_++;
	}
	else
	{
		viewProjection_->target_.z = -beforeTargetNum_.z;
	}
	//カメラ移動処理
	if (isPassPoint_ == false)
	{
		viewProjection_->eye_.x = beforeMoveCameraPos_.x + (cameraMovePoint_.x - beforeMoveCameraPos_.x) * MathFunc::easeInSine(cameraMoveTimer_ / cameraMoveTimerMax_);
		viewProjection_->eye_.z = beforeMoveCameraPos_.z + cameraMoveZ_ * MathFunc::easeInSine(cameraMoveTimer_ / cameraMoveTimerMax_);
		cameraMoveTimer_++;
		if (cameraMoveTimer_ > cameraMoveTimerMax_)
		{
			isPassPoint_ = true;
			cameraMoveTimer_ = 0.0f;
		}
	}
	else
	{
		viewProjection_->eye_.x = cameraMovePoint_.x + -cameraMovePoint_.x * MathFunc::easeOutSine(cameraMoveTimer_ / cameraMoveTimerMax_);
		viewProjection_->eye_.z = cameraMovePoint_.z + cameraMoveZ_ * MathFunc::easeOutSine(cameraMoveTimer_ / cameraMoveTimerMax_);
		cameraMoveTimer_++;
		if (cameraMoveTimer_ > cameraMoveTimerMax_)
		{
			//位置のenumを切り替え
			if (cameraPos_ == CameraPos::Back)
			{
				cameraPos_ = Front;
				red_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z - 1.0f));
				black_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z - 2.0f));
			}
			else
			{
				cameraPos_ = Back;
				red_->SetPosition(Vector3(0.0f, viewProjection_->eye_.y, viewProjection_->eye_.z + 1.0f));
			}
			isPassPoint_ = false;
			cameraMoveTimer_ = 0.0f;
			isChangeCameraDir_ = false;
			targetMoveTimer_ = 0.0f;
			viewProjection_->eye_.z = cameraMoveZ_;
		}
	}
}

void GamePlayScene::MoveCamera()
{
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

	//モデル読み込み
	modelStardust = Model::LoadFromOBJ("stardust");

	stardustModels_.insert(std::make_pair("stardust", modelStardust));

	// レベルデータからオブジェクトを生成、配置
	//星屑
	for (auto& objectData : backGroundStardust_->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(stardustModels_)::iterator it = stardustModels_.find(objectData.fileName);
		if (it != stardustModels_.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		stardust = new Stardust;
		stardust->StardustInitialize();
		stardust->SetModel(model);

		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		stardust->SetPosition(pos);
		//初期Y座標を保存しておく
		stardust->SetBeforeY(pos.y);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		stardust->SetRotation(rot);

		stardust->SetSize();

		// 配列に登録
		stardustObjects_.push_back(stardust);
	}
}

void GamePlayScene::UIInitialize()
{
	UIInitPos_ = 0.0f;
	UIInitRange_ = 100.0f;
	UIInitTime_ = 0.0f;
	UIOutPos_ = 0.0f;
	UIOutRange_ = 100.0f;
	UIOutTime_ = 0.0f;
	UIMovePos_ = 0.0f;
	UIMoveRange_ = 5.0f;
	UIMoveTime_ = 0.0f;

	isUIInit_ = false;
	isUIOut_ = false;
	isNeutral_ = true;
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;

	arrowUpOn = new Sprite;
	arrowUpOn->Initialize(dxCommon_);
	arrowUpOn->LoadTexture(0, L"Resources/2d/offUp.png", dxCommon_);
	arrowUpOn->SetScale({ 0.8f,0.8f });
	arrowUpOn->SetPosition({ 600,0 - UIInitRange_,0 });
	arrowUpOff = new Sprite;
	arrowUpOff->Initialize(dxCommon_);
	arrowUpOff->LoadTexture(0, L"Resources/2d/onUp.png", dxCommon_);
	arrowUpOff->SetScale({ 0.8f,0.8f });
	arrowUpOff->SetPosition({ 600,0 - UIInitRange_,0 });
	arrowDownOn = new Sprite;
	arrowDownOn->Initialize(dxCommon_);
	arrowDownOn->LoadTexture(0, L"Resources/2d/offDown.png", dxCommon_);
	arrowDownOn->SetScale({ 0.8f,0.8f });
	arrowDownOn->SetPosition({ 600,640 + UIInitRange_,0 });
	arrowDownOff = new Sprite;
	arrowDownOff->Initialize(dxCommon_);
	arrowDownOff->LoadTexture(0, L"Resources/2d/onDown.png", dxCommon_);
	arrowDownOff->SetScale({ 0.8f,0.8f });
	arrowDownOff->SetPosition({ 600,640 + UIInitRange_,0 });
	arrowRightOn = new Sprite;
	arrowRightOn->Initialize(dxCommon_);
	arrowRightOn->LoadTexture(0, L"Resources/2d/offRight.png", dxCommon_);
	arrowRightOn->SetScale({ 0.8f,0.8f });
	arrowRightOn->SetPosition({ 1200 + UIInitRange_,300,0 });
	arrowRightOff = new Sprite;
	arrowRightOff->Initialize(dxCommon_);
	arrowRightOff->LoadTexture(0, L"Resources/2d/onRight.png", dxCommon_);
	arrowRightOff->SetScale({ 0.8f,0.8f });
	arrowRightOff->SetPosition({ 1200 + UIInitRange_,300,0 });
	arrowLeftOn = new Sprite;
	arrowLeftOn->Initialize(dxCommon_);
	arrowLeftOn->LoadTexture(0, L"Resources/2d/offLeft.png", dxCommon_);
	arrowLeftOn->SetScale({ 0.8f,0.8f });
	arrowLeftOn->SetPosition({ 0 - UIInitRange_,300,0 });
	arrowLeftOff = new Sprite;
	arrowLeftOff->Initialize(dxCommon_);
	arrowLeftOff->LoadTexture(0, L"Resources/2d/onLeft.png", dxCommon_);
	arrowLeftOff->SetScale({ 0.8f,0.8f });
	arrowLeftOff->SetPosition({ 0 - UIInitRange_,300,0 });
}

void GamePlayScene::UIInitMotion()
{
	if (isUIInit_ == false && isPlayerInit_ == true)
	{
		UIInitPos_ = UIInitRange_ * MathFunc::easeOutSine(UIInitTime_ / 30.0f);
		UIInitTime_++;
		arrowUpOn->SetPosition({ 600,0 - UIInitRange_ + UIInitPos_,0 });
		arrowUpOff->SetPosition({ 600,0 - UIInitRange_ + UIInitPos_,0 });
		arrowDownOn->SetPosition({ 600,640 + UIInitRange_ - UIInitPos_,0 });
		arrowDownOff->SetPosition({ 600,640 + UIInitRange_ - UIInitPos_,0 });
		arrowRightOn->SetPosition({ 1200 + UIInitRange_ - UIInitPos_,300,0 });
		arrowRightOff->SetPosition({ 1200 + UIInitRange_ - UIInitPos_,300,0 });
		arrowLeftOn->SetPosition({ 0 - UIInitRange_ + UIInitPos_,300,0 });
		arrowLeftOff->SetPosition({ 0 - UIInitRange_ + UIInitPos_,300,0 });
	}
	if (UIInitPos_ >= UIInitRange_)
	{
		isUIInit_ = true;
	}
}

void GamePlayScene::UIOutMotion()
{
	isNeutral_ = false;
	if (isUIOut_ == false)
	{
		if (UIOutPos_ < UIOutRange_)
		{
			UIOutPos_ = UIOutRange_ * MathFunc::easeOutSine(UIOutTime_ / 30.0f);
			UIOutTime_++;
			arrowUpOn->SetPosition({ 600.0f,0.0f - UIOutPos_,0.0f });
			arrowUpOff->SetPosition({ 600.0f,0.0f - UIOutPos_,0.0f });
			arrowDownOn->SetPosition({ 600.0f,640.0f + UIOutPos_,0.0f });
			arrowDownOff->SetPosition({ 600.0f,640.0f + UIOutPos_,0.0f });
			arrowRightOn->SetPosition({ 1200.0f + UIOutPos_,300.0f,0.0f });
			arrowRightOff->SetPosition({ 1200.0f + UIOutPos_,300.0f,0.0f });
			arrowLeftOn->SetPosition({ 0.0f - UIOutPos_,300.0f,0.0f });
			arrowLeftOff->SetPosition({ 0.0f - UIOutPos_,300.0f,0.0f });
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
	UIMove();
	arrowUpOn->Update();
	arrowUpOff->Update();
	arrowDownOn->Update();
	arrowDownOff->Update();
	arrowRightOn->Update();
	arrowRightOff->Update();
	arrowLeftOn->Update();
	arrowLeftOff->Update();
}

void GamePlayScene::UIDraw()
{
	//移動時のUI切り替え
	if (isUp_ == true)
	{
		arrowUpOn->SetTextureCommands(0, dxCommon_);
		arrowUpOn->Draw(dxCommon_);
	}
	else
	{
		arrowUpOff->SetTextureCommands(0, dxCommon_);
		arrowUpOff->Draw(dxCommon_);
	}
	if (isDown_ == true)
	{
		arrowDownOn->SetTextureCommands(0, dxCommon_);
		arrowDownOn->Draw(dxCommon_);
	}
	else
	{
		arrowDownOff->SetTextureCommands(0, dxCommon_);
		arrowDownOff->Draw(dxCommon_);
	}
	if (isRight_ == true)
	{
		arrowRightOn->SetTextureCommands(0, dxCommon_);
		arrowRightOn->Draw(dxCommon_);
	}
	else
	{
		arrowRightOff->SetTextureCommands(0, dxCommon_);
		arrowRightOff->Draw(dxCommon_);
	}
	if (isLeft_ == true)
	{
		arrowLeftOn->SetTextureCommands(0, dxCommon_);
		arrowLeftOn->Draw(dxCommon_);
	}
	else
	{
		arrowLeftOff->SetTextureCommands(0, dxCommon_);
		arrowLeftOff->Draw(dxCommon_);
	}
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
		arrowUpOn->SetPosition({ 600,-UIMovePos_,0 });
		arrowUpOff->SetPosition({ 600,-UIMovePos_,0 });
		arrowDownOn->SetPosition({ 600,640 + UIMovePos_,0 });
		arrowDownOff->SetPosition({ 600,640 + UIMovePos_,0 });
		arrowRightOn->SetPosition({ 1200 + UIMovePos_,300,0 });
		arrowRightOff->SetPosition({ 1200 + UIMovePos_,300,0 });
		arrowLeftOn->SetPosition({ -UIMovePos_ ,300,0 });
		arrowLeftOff->SetPosition({ -UIMovePos_ ,300,0 });
	}
}
