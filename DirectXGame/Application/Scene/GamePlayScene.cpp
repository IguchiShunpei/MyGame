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

	//背景オブジェクト
	backGround_ = std::make_unique <BackGround>();
	backGround_->Initialize();

	//player 
	player_ = std::make_unique <Player>();
	player_->PlayerInitialize();
	player_->LaserInitialize();

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

	//メンバ変数の初期化

	//ゲーム中のシーン番号
	gameNum_ = GameNum::FirstScene;

	gameClearmoveZ_ = 0.5f;
	gameClearPosZ_ = 100.0f;
	clearCameraMoveZ_ = 0.05f;

	//シェイク
	playerCameraShake_ = 0.5f;
	bossCameraShake_ = 1.5f;
	enemyCameraShake_ = 0.5f;

	//ゲームオーバーシーンに遷移する時間
	gameOverTimer_ = 0;
	//最大
	gameOverTimerMax_ = 60;

	//フラグ
	isWait_ = false;
	isClearScene_ = false;
	isBossEffect_ = false;
	isBEnemyDeadScene_ = false;
	isBossInitCamera_ = false;
	isStart_ = false;
	isDeadCameraShake_ = false;
	isTutorial_ = true;
	isEndTutorial_ = false;

	//タイマー
	waitTimer_ = 0;
	hitPlayerTimer_ = 0;
	hitPlayerTimerMax_ = 32;

	//スコア
	score_ = 0;
}

void GamePlayScene::Update()
{
	DeleteObject();

	if (isTutorial_ == false)
	{
		//更新コマンド
		UpdateEnemyPop();
	}

	//天球
	sky_->worldTransform_.UpdateMatrix();

	//背景オブジェクト
	backGround_->Update();

	//黒フェードアウト
	ui_->FadeOut(ui_->Black);

	//自機登場演出
	PlayerInit();

	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		meteors->SetDamage(player_->GetBulletPower());
		meteors->MeteorUpdate();
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
		//移動するだけの敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->WEnemyUpdate();
			wEnemys->SetDamage(player_->GetBulletPower());
		}
		//倒せない敵
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->InvincibleEnemyUpdate(player_->GetPosition());
		}
		//弾を撃つ敵
		for (std::unique_ptr<ShotEnemy>& enemys : sEnemys_)
		{
			enemys->ShotEnemyUpdate(player_->GetPosition());
			enemys->SetDamage(player_->GetBulletPower());
		}
		break;

	case BossScene://ボス戦専用の処理
		if (isBossInitCamera_ == true)
		{
			//ボスの登場演出
			BossInit();
		}
		//ボス登場演出フラグがfalseになったらボス戦開始
		else
		{
			//更新
			if (bEnemy_->GetIsDeathTimer() == false)
			{
				bEnemy_->BossUpdate();
				bEnemy_->SetDamage(player_->GetBulletPower());
				bEnemy_->PhaseChange(player_->GetPosition());
			}
			else
			{
				isBEnemyDeadScene_ = true;
				//タイマーを動かす処理
				bEnemy_->ActiveDeathTimer();
				//死亡演出
				BossDead();
				if (isClearScene_ == false)
				{
					//カメラシェイク
					camera_->CameraShake(bEnemy_->GetDeathTimer(), enemyCameraShake_, enemyCameraShake_);
				}
			}
			if (isClearScene_ == false)
			{
				//死亡フラグがtrueになったら
				if (bEnemy_->GetIsDead() == true)
				{
					score_ += bEnemy_->GetScore();
					isClearScene_ = true;
					camera_->SetIsShake(false);
					normalEyeNum_ = camera_->GetViewProjection()->eye_;
					normalTargetNum_ = camera_->GetViewProjection()->target_;
					camera_->GetViewProjection()->SetTarget(player_->GetPosition());
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
		player_->GetLaser()->LaserUpdate(player_->GetPosition(), player_->GetRotation());
		if (isBossInitCamera_ == false && isClearScene_ == false && isBEnemyDeadScene_ == false)
		{
			camera_->MoveCamera();

			player_->Update();
			player_->ColliderUpdate();
			player_->GetLaser()->LaserUpdate(player_->GetPosition(), player_->GetRotation());
		}
		else
		{
			player_->BulletUpdate();
			bEnemy_->BulletUpdate();
		}
		Tutorial();
		if (isEndTutorial_ == true)
		{
			ui_->EndTutorial(isTutorial_);
		}
		//UI更新
		ui_->ScoreCalc(score_);
		ui_->UIUpdate();
	}

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	//もし攻撃に当たって無かったら
	if (player_->GetIsDead() == false)
	{
		if (player_->GetIsHit() == true)
		{
			//攻撃を受けた時の無敵フラグがtrueになったら
			if (player_->GetIsInv() == true)
			{
				if (hitPlayerTimer_ <= hitPlayerTimerMax_)
				{
					ui_->SetIsRed(true);
					ui_->FadeOut(ui_->Red);
					camera_->CameraShake(hitPlayerTimer_, playerCameraShake_, playerCameraShake_);
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
					camera_->SetIsShake(false);
					hitPlayerTimer_ = 0;
				}
			}
		}
	}

	//敵に弾が当たった時のカメラシェイク
	if (isDeadCameraShake_ == true)
	{
		hitEnemyTimer_++;
		if (hitEnemyTimer_ < 16)
		{
			camera_->CameraShake(hitEnemyTimer_, enemyCameraShake_, enemyCameraShake_);
		}
		else
		{
			hitEnemyTimer_ = 0;
			camera_->SetIsShake(false);
			isDeadCameraShake_ = false;
		}
	}

	//エフェクト更新
	EffectUpdate(isDeadCameraShake_);

	//ゲームオーバー演出
	ToGameOverScene();

	//カメラ
	camera_->Update();

	//スコア
	for (std::unique_ptr<Score>& scoreObjects : scoreObjects_)
	{
		scoreObjects->ScoreUpdate();
	}
}

void GamePlayScene::Draw()
{
	//描画前処理
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	//天球
	sky_->Draw(camera_->GetViewProjection());

	//背景オブジェクト
	backGround_->Draw(camera_->GetViewProjection());

	//スコア
	for (std::unique_ptr<Score>& scoreObjects : scoreObjects_)
	{
		scoreObjects->Draw(camera_->GetViewProjection(), scoreObjects->GetAlpha(), scoreObjects->GetColor());
	}

	//隕石(敵)
	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		if (meteors->GetIsDelete() == false)
		{
			meteors->Draw(camera_->GetViewProjection(), meteors->GetAlpha(), meteors->GetColor());
		}
	}
	//アイテム
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
		for (std::unique_ptr<ShotEnemy>& enemys : sEnemys_)
		{
			enemys->Draw(camera_->GetViewProjection(), 1.0f, enemys->GetColor());

			enemys->BulletDraw(camera_->GetViewProjection());
		}
		break;

	case BossScene:
		//ボス敵
		if (bEnemy_->GetIsDead() == false)
		{
			bEnemy_->Draw(camera_->GetViewProjection(), bEnemy_->GetAlpha(), bEnemy_->GetColor());
			bEnemy_->BulletDraw(camera_->GetViewProjection());
		}
		break;
	}

	//プレイヤー
	if (isPlayerDead_ == false)
	{
		player_->Draw(camera_->GetViewProjection());
		player_->BulletDraw(camera_->GetViewProjection());
		player_->GetLaser()->Draw(camera_->GetViewProjection(), player_->GetLaser()->GetAlpha(), player_->GetLaser()->GetBulletColor());
	}

	Object3d::PostDraw();

	//エフェクト
	effect_->Draw(isBEnemyDeadScene_, isPlayerDead_);

	//UI
	ui_->UIDraw();


	// 描画後処理
	dxCommon_->PostDraw();
}

void GamePlayScene::Finalize()
{

}

void GamePlayScene::EffectUpdate(bool& isDeadCameraShake)
{
	//雑魚敵死亡時のパーティクル
	for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
	{
		effect_->W_ParticleUpdate(wEnemys.get(), isDeadCameraShake);
		WEnemyDead(wEnemys.get());
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
	{
		effect_->H_ParticleUpdate(invEnemys.get());
	}

	//弾を撃つ敵の死亡処理
	for (std::unique_ptr<ShotEnemy>& sEnemys : sEnemys_)
	{
		effect_->E_ParticleUpdate(sEnemys.get(), isDeadCameraShake);
		SEnemyDead(sEnemys.get());
	}

	//隕石のパーティクル
	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		effect_->M_ParticleUpdate(meteors.get(), isDeadCameraShake);
		MeteorDead(meteors.get());
	}

	//ボス敵の被ダメージ処理
	if (gameNum_ == GameNum::BossScene)
	{
		if (bEnemy_->GetIsInit() == true && isBEnemyDeadScene_ == true)
		{
			effect_->B_ParticleUpdate(bEnemy_.get(), isDeadCameraShake);
		}
	}

	//全て更新
	effect_->Update();
}

void GamePlayScene::LoadEnemyPop()
{
	sEnemys_.clear();
	wEnemys_.clear();
	invincibleEnemys_.clear();

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/EnemyPop.csv");
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
			else if (word.find("U") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->U);
				newWEnemy->SetRotation({ 0.0f,0.0f,90.0f });
			}
			else if (word.find("D") == 0)
			{
				newWEnemy->SetPhase(newWEnemy->D);
				newWEnemy->SetRotation({ 0.0f,0.0f,90.0f });
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

			newWEnemy->SetScale(Vector3(2.0f, 2.0f, 2.0f));
			newWEnemy->worldTransform_.UpdateMatrix();
			//登録
			wEnemys_.push_back(std::move(newWEnemy));
		}
		// enemy
		else if (key == "enemy") {
			std::string word;
			std::string direction;
			getline(line_stream, word, ' ');
			//敵の生成
			std::unique_ptr<ShotEnemy> newEnemy = std::make_unique<ShotEnemy>();
			//敵の初期化
			newEnemy->ShotEnemyInitialize();
			//コライダーの追加
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 3.0f));
			if (word.find("HP") == 0)
			{
				std::string num;
				getline(line_stream, num, ' ');
				//hpを保存
				int32_t hpNum = atoi(num.c_str());
				newEnemy->SetHp(hpNum);
			}
			getline(line_stream, direction, ' ');
			//移動向きを読み取ってセットする
			if (direction.find("U") == 0)
			{
				newEnemy->SetMove(newEnemy->Up);
			}
			else if (direction.find("D") == 0)
			{
				newEnemy->SetMove(newEnemy->Down);
			}
			else if (direction.find("R") == 0)
			{
				newEnemy->SetMove(newEnemy->Right);
			}
			else if (direction.find("L") == 0)
			{
				newEnemy->SetMove(newEnemy->Left);
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
			newEnemy->SetScale(Vector3(3.0f, 3.0f, 3.0f));
			newEnemy->worldTransform_.UpdateMatrix();
			//登録
			sEnemys_.push_back(std::move(newEnemy));
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
			newInvincibleEnemy->SetScale(Vector3(2.5f, 2.5f, 2.5f));
			newInvincibleEnemy->worldTransform_.UpdateMatrix();
			//登録
			invincibleEnemys_.push_back(std::move(newInvincibleEnemy));
		}
		//meteor
		else if (key == "meteor") {

			//敵の生成
			std::unique_ptr<Meteor> newMeteor = std::make_unique<Meteor>();
			//敵の初期化
			newMeteor->MeteorInitialize();
			//コライダーの追加
			newMeteor->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			std::string deadEffect;

			getline(line_stream, deadEffect, ' ');

			//移動向きを読み取ってセットする
			if (deadEffect.find("I") == 0)
			{
				newMeteor->SetDeadEffect(newMeteor->Item);
			}
			else if (deadEffect.find("M") == 0)
			{
				newMeteor->SetDeadEffect(newMeteor->SmallMeteor);
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
			//bossEnemy
			bEnemy_ = std::make_unique < BossEnemy>();
			bEnemy_->BossEnemyInitialize();
			bEnemy_->BossUpdate();
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
	sEnemys_.remove_if([](std::unique_ptr <ShotEnemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	sEnemys_.remove_if([](std::unique_ptr <ShotEnemy>& enemy)
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
	scoreObjects_.remove_if([](std::unique_ptr <Score>& scoreObject)
		{
			return scoreObject->GetIsDelete();
		});
}

void GamePlayScene::PlayerInit()
{
	if (isPlayerInit_ == false)
	{
		//カメラワーク
		camera_->PlayerInitCameraWork(isStart_, player_.get());
		//自機の登場モーション
		player_->InitMotion();
		//カメラワークが終わったらフラグtrue
		if (isStart_ == true)
		{
			isPlayerInit_ = true;
		}
	}
}

void GamePlayScene::PlayerDead()
{
	effect_->Ex_ParticleUpdate(player_.get(), isPlayerDead_);
}

void GamePlayScene::BossInit()
{
	if (isBossInitCamera_ == true)
	{
		ui_->Stop();
		camera_->BossInitCameraWork(bEnemy_.get(), isBossInitCamera_);
	}
}

void GamePlayScene::BossDead()
{
	//UI退場
	ui_->UIOutMotion();
	//ボスの爆発
	effect_->GetExplosion01()->EnemyExplosionUpdate();
	effect_->GetExplosion02()->EnemyExplosionUpdate();
	bEnemy_->Dead();
}
void GamePlayScene::ToClearScene()
{
	if (isClearScene_ == true)
	{
		if (isClearCameraWork_ == false)
		{
			//カメラワーク
			camera_->ToClearCameraWork(isClearCameraWork_);
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
			camera_->GetViewProjection()->eye_.z -= gameClearmoveZ_;
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
		camera_->ToGameOverCameraWork(player_.get(), isPlayerDead_, isGameOver_);
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
			else
			{
				//カメラシェイク
				camera_->CameraShake(180, enemyCameraShake_, enemyCameraShake_);//カメラシェイク
			}
		}
	}
	else
	{
		deadCameraPos_ = camera_->GetViewProjection()->GetEye();
	}
}

void GamePlayScene::Tutorial()
{
		//Bキーを押したとき
		if (input_->TriggerKey(DIK_B))
		{
			isEndTutorial_ = true;
		}
}

void GamePlayScene::WEnemyDead(WeakEnemy* wEnemy)
{
	if (wEnemy->GetIsDead() == true)
	{
		score_ += wEnemy->GetScore();
		//敵の生成
		std::unique_ptr<Score> scoreWeakEnemy = std::make_unique<Score>();
		//スコアオブジェクトを隕石にする
		scoreWeakEnemy->SetEnemyObject(scoreWeakEnemy->WEAKENEMY);
		//敵の初期化
		scoreWeakEnemy->ScoreInitialize();
		//登録
		scoreObjects_.push_back(std::move(scoreWeakEnemy));
	}
}

void GamePlayScene::SEnemyDead(ShotEnemy* sEnemy)
{
	if (sEnemy->GetIsDead() == true)
	{
		score_ += sEnemy->GetScore();
		//敵の生成
		std::unique_ptr<Score> scoreShotEnemy = std::make_unique<Score>();
		//スコアオブジェクトを隕石にする
		scoreShotEnemy->SetEnemyObject(scoreShotEnemy->SHOTENEMY);
		//敵の初期化
		scoreShotEnemy->ScoreInitialize();
		//登録
		scoreObjects_.push_back(std::move(scoreShotEnemy));
	}
}

void GamePlayScene::MeteorDead(Meteor* meteor)
{
	if (meteor->GetIsDead() == true)
	{
		score_ += meteor->GetScore();
		//敵の生成
		std::unique_ptr<Score> scoreMeteor = std::make_unique<Score>();
		//スコアオブジェクトを隕石にする
		scoreMeteor->SetEnemyObject(scoreMeteor->METEOR);
		//敵の初期化
		scoreMeteor->ScoreInitialize();
		//登録
		scoreObjects_.push_back(std::move(scoreMeteor));

		if (meteor->GetDeadEffect() == meteor->Item)
		{
			//敵の生成
			std::unique_ptr<Item> newItem = std::make_unique<Item>();
			//敵の初期化
			newItem->ItemInitialize(meteor->GetPosition());
			//コライダーの追加
			newItem->SetCollider(new SphereCollider(Vector3(0, 0, 0), 2.5f));
			//登録
			items_.push_back(std::move(newItem));
		}
		else if (meteor->GetDeadEffect() == meteor->SmallMeteor)
		{
			//3つ出す
			for (int i = 0; i < 3; i++)
			{
				//敵の生成
				std::unique_ptr<Meteor> newMeteor = std::make_unique<Meteor>();
				//敵の初期化
				newMeteor->MeteorInitialize();
				//コライダーの追加
				newMeteor->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));
				// 座標データに追加
				newMeteor->SetPosition(meteor->GetPosition() + meteor->GetSmallMeteorPos(i));
				newMeteor->SetVelocity(meteor->GetVelocity(i));
				newMeteor->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				newMeteor->SetAlpha(1.0f);
				newMeteor->SetDeadEffect(newMeteor->None);
				newMeteor->worldTransform_.UpdateMatrix();

				//登録
				meteors_.push_back(std::move(newMeteor));
			}
		}
	}
}
