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
	input_ = SIEngine::Input::GetInstance();
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	//当たり判定
	collisionManager_ = CollisionManager::GetInstance();

	//天球
	sky_ = std::make_unique < SkyDome>();
	sky_->SkyDomeInitialize();

	//player 
	player_ = std::make_unique <Player>();
	player_->PlayerInitialize();

	//bossEnemy
	bEnemy = std::make_unique < BossEnemy>();
	bEnemy->BossEnemyInitialize();
	bEnemy->worldTransform_.UpdateMatrix();
	bEnemy->ColliderUpdate();

	//敵ファイル読み込み
	LoadEnemyPop();

	//UIの初期化
	ui_ = std::make_unique<UI>();
	ui_->UIInitialize();

	//カメラ
	camera_ = std::make_unique < Camera>();
	camera_->Initialize();
	camera_->SetUI(ui_.get());
	camera_->SetPlayer(player_.get());

	//エフェクト
	effect_ = std::make_unique < Effect>();
	effect_->SetView(camera_->GetViewProjection());
	effect_->Initialize();
	effect_->SetPlayer(player_.get());

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
			if (player_->GetBulletLevel() < 2)
			{
				player_->SetBulletLevel(player_->GetBulletLevel() + 1);
			}
		}
	}

	switch (gameNum_)
	{
	case FirstScene://雑魚敵戦専用の処理

		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->SetDamage(player_->GetBulletPower());
			wEnemys->ColliderUpdate();
		}
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Update(player_->GetPosition());
			invEnemys->ColliderUpdate();
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Update(player_->GetPosition());
			enemys->SetDamage(player_->GetBulletPower());
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
				bEnemy->SetDamage(player_->GetBulletPower());
				bEnemy->PhaseChange(player_->GetPosition());
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

				}
			}
			if (isClearScene_ == false)
			{
				//死亡フラグがtrueになったら
				if (bEnemy->GetIsDead() == true)
				{
				}
				else
				{

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
			camera_->MoveCamera();

			player_->Update();
			player_->ColliderUpdate();
		}
		else
		{
			player_->BulletUpdate();
			bEnemy->BulletUpdate();
		}
		//UI更新
		ui_->UIUpdate();
	}

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	//もし攻撃に当たって無かったら
	if (player_->GetIsDead() == false)
	{
		if (player_->GetIsHit() == false)
		{
		
		}
		else
		{
			//攻撃を受けた時の無敵フラグがtrueになったら
			if (player_->GetIsInv() == true)
			{
				if (hitPlayerTimer_ <= hitPlayerTimerMax_)
				{
					ui_->SetIsRed(true);
					ui_->FadeOut(ui_->Red);
					ui_->Damage((float)hitPlayerTimer_, (float)hitPlayerTimerMax_, player_->GetHp(), player_->GetHpMax());
					//無敵時間
					hitPlayerTimer_++;
				}
				else
				{
					ui_->SetIsRed(false);
					ui_->RedReset();
					player_->SetIsHit(false);
					player_->SetIsInv(false);
					hitPlayerTimer_ = 0;
				}
			}
		}
	}

	//エフェクト更新
	EffectUpdate();

	ToGameOverScene();

	//カメラ
	camera_->Update();
}

void GamePlayScene::Draw()
{
	//描画前処理
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky_->Draw(camera_->GetViewProjection());
	if (isPlayerDead_ == false)
	{
		player_->Draw(camera_->GetViewProjection());
		player_->BulletDraw(camera_->GetViewProjection());
	}

	//星屑
	for (auto& object : stardustObjects_) {
		object->Draw(camera_->GetViewProjection(), 1.0f, object->GetColor());
	}
	//隕石
	for (auto& object : meteorObjects_) {
		object->Draw(camera_->GetViewProjection(), object->GetAlpha(), object->GetColor());
	}

	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		if (meteors->GetIsDelete() == false)
		{
			meteors->Draw(camera_->GetViewProjection(), meteors->GetAlpha(), meteors->GetColor());
		}
	}
	for (std::unique_ptr<Item>& items : items_)
	{
		items->Draw(camera_->GetViewProjection());
	}
	switch (gameNum_)
	{
	case FirstScene:
		//雑魚敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Draw(camera_->GetViewProjection(), 1.0f, wEnemys->GetColor());
		}
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Draw(camera_->GetViewProjection());
		}
		//敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Draw(camera_->GetViewProjection(), 1.0f, enemys->GetColor());

			enemys->BulletDraw(camera_->GetViewProjection());
		}
		break;

	case BossScene:
		//ボス敵
		if (bEnemy->GetIsDead() == false)
		{
			bEnemy->Draw(camera_->GetViewProjection(), bEnemy->GetAlpha(), bEnemy->GetColor());
			bEnemy->BulletDraw(camera_->GetViewProjection());
		}
		break;
	}

	Object3d::PostDraw();

	//エフェクト
	effect_->Draw();

	//UI
	ui_->UIDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void GamePlayScene::Finalize()
{

}

void GamePlayScene::EffectUpdate()
{
	//雑魚敵死亡時のパーティクル
	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		effect_->W_ParticleUpdate(wEnemys.get());
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
	{
		effect_->H_ParticleUpdate(invEnemys.get());
	}

	//敵の被ダメージ処理
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		effect_->E_ParticleUpdate(enemys.get());
	}

	//隕石のパーティクル
	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		effect_->M_ParticleUpdate(meteors.get());
	}
	//ボス敵の被ダメージ処理
	if (bEnemy->GetIsInit() == true)
	{
		if (isBEnemyDeadScene_ == true)
		{
			effect_->B_ParticleUpdate(bEnemy.get());
		}
	}

	//全て更新
	effect_->Update();
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

		//自機の登場モーション
		player_->InitMotion();
		//カメラワークが終わったらフラグtrue
			isPlayerInit_ = true;
	}
}

void GamePlayScene::PlayerDead()
{
	effect_->Ex_ParticleUpdate(player_.get(),isPlayerDead_);
}

void GamePlayScene::BossInit()
{
	if (isBossInitCamera_ == true)
	{
		ui_->Stop();
	}
}

void GamePlayScene::BossDead()
{
	//UI退場
	ui_->UIOutMotion();
	effect_->GetExplosion01()->EnemyExplosionUpdate();
	effect_->GetExplosion02()->EnemyExplosionUpdate();
	bEnemy->Dead();
}
void GamePlayScene::ToClearScene()
{
	if (isClearScene_ == true)
	{
		if (isClearCameraWork_ == false)
		{

		}
		else
		{
			//黒フェードイン
			ui_->SetIsBlack(false);
			ui_->FadeIn();
			//自機を動かす
			player_->worldTransform_.rotation_.z++;
			player_->worldTransform_.position_.x -= clearCameraMoveZ_;
			player_->worldTransform_.position_.z++;
			// ワールドトランスフォームの行列更新と転送
			player_->worldTransform_.UpdateMatrix();
			//自機が移動しきったらシーン遷移
			if (player_->worldTransform_.position_.z >= gameClearPosZ_)
			{
				GameSceneManager::GetInstance()->ChangeScene("CLEAR");
				isClearScene_ = false;
			}
		}
	}
}

void GamePlayScene::ToGameOverScene()
{
	if (player_->GetIsDead() == true)
	{
		//UI退場
		ui_->UIOutMotion();
		//カメラワーク

		//自機死亡演出
		PlayerDead();
		player_->worldTransform_.UpdateMatrix();
		if (isGameOver_ == true)
		{
			effect_->GetExplosion03()->PlayerExplosionUpdate(player_->GetPosition());
			if (effect_->GetExplosion03()->GetIsFinish() == true)
			{
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

	}
}


void GamePlayScene::LoadLevelData()
{
	//背景オブジェクトデータの読み込み
	backGroundStar_ = LevelLoader::LoadFile("stardust");
	backGroundMeteor_ = LevelLoader::LoadFile("meteor");

	// レベルデータからオブジェクトを生成、配置
	//星屑
	for (auto& objectData : backGroundStar_->objects) {
		//モデル読み込み
		std::unique_ptr<Model>backModelStardust = Model::LoadFromOBJ("stardust");
		stardustModels_.insert(std::make_pair("stardust", move(backModelStardust)));
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Stardust> newObject = std::make_unique<Stardust>();
		newObject->StardustInitialize();
		newObject->SetModel(backModelStardust.get());
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
		std::unique_ptr<Model>backModelMeteor = Model::LoadFromOBJ("bigMeteor");
		meteorModels_.insert(std::make_pair("meteor",move(backModelMeteor)));
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Meteor> newObject = std::make_unique<Meteor>();;
		newObject->MeteorInitialize();
		newObject->SetModel(backModelMeteor.get());
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