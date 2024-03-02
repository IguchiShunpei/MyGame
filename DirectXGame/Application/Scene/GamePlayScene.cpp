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

	//メンバ変数の初期化

	//ゲーム中のシーン番号
	gameNum_ = GameNum::FirstScene;

	gameClearmoveZ_ = 0.5f;
	gameClearPosZ_ = 100.0f;
	clearCameraMoveZ_ = 0.05f;

	//シェイク
	playerCameraShake_ = 0.2f;
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

	//タイマー
	waitTimer_ = 0;
	hitPlayerTimer_ = 0;
	hitPlayerTimerMax_ = 32;
}

void GamePlayScene::Update()
{
	DeleteObject();

	//更新コマンド
	UpdateEnemyPop();

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
		//移動するだけの敵
		for (std::unique_ptr<WeakEnemy>& wEnemys : wEnemys_)
		{
			wEnemys->Update();
			wEnemys->SetDamage(player_->GetBulletPower());
			wEnemys->ColliderUpdate();
		}
		//倒せない敵
		for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
		{
			invEnemys->Update(player_->GetPosition());
			invEnemys->ColliderUpdate();
		}
		//弾を撃つ敵
		for (std::unique_ptr<Enemy>& enemys : enemys_)
		{
			enemys->Update(player_->GetPosition());
			enemys->SetDamage(player_->GetBulletPower());
			enemys->ColliderUpdate();
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
					//deathTimerが奇数なら
					if (bEnemy->GetDeathTimer() % 2 != 1)
					{
						camera_->CameraShake(enemyCameraShake_, enemyCameraShake_);
					}
					else
					{
						cameraShakePos_ = camera_->GetViewProjection()->GetEye();
					}
				}
			}
			if (isClearScene_ == false)
			{
				//死亡フラグがtrueになったら
				if (bEnemy->GetIsDead() == true)
				{
					camera_->GetViewProjection()->SetEye(cameraShakePos_);
					isClearScene_ = true;
					normalEyeNum_ = camera_->GetViewProjection()->eye_;
					normalTargetNum_ = camera_->GetViewProjection()->target_;
					camera_->GetViewProjection()->SetTarget(player_->GetPosition());
				}
				else
				{
					cameraShakePos_ = camera_->GetViewProjection()->GetEye();
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
		player_->LaserUpdate();
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
			cameraShakePos_ = camera_->GetViewProjection()->GetEye();
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
					if (hitPlayerTimer_ % 2 != 1)
					{
						camera_->CameraShake(playerCameraShake_, playerCameraShake_);
					}
					else
					{
						camera_->GetViewProjection()->SetEye(cameraShakePos_);
					}
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

	//敵に弾が当たった時のカメラシェイク
	if (isDeadCameraShake_ == true)
	{
		hitEnemyTimer_++;
		if (hitEnemyTimer_ < 16)
		{
			if (hitEnemyTimer_ % 2 != 1)
			{
				camera_->CameraShake(enemyCameraShake_, enemyCameraShake_);
			}
			else
			{
				camera_->GetViewProjection()->SetEye(cameraShakePos_);
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
		cameraShakePos_ = camera_->GetViewProjection()->GetEye();
	}

	//エフェクト更新
	EffectUpdate(isDeadCameraShake_);

	//ゲームオーバー演出
	ToGameOverScene();

	//カメラ
	camera_->Update();
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

	//プレイヤー
	if (isPlayerDead_ == false)
	{
		player_->Draw(camera_->GetViewProjection());
		player_->BulletDraw(camera_->GetViewProjection());
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
	}
	//無敵敵に弾が当たった時
	for (std::unique_ptr<InvincibleEnemy>& invEnemys : invincibleEnemys_)
	{
		effect_->H_ParticleUpdate(invEnemys.get());
	}

	//敵の被ダメージ処理
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		effect_->E_ParticleUpdate(enemys.get(), isDeadCameraShake);
	}

	//隕石のパーティクル
	for (std::unique_ptr<Meteor>& meteors : meteors_)
	{
		effect_->M_ParticleUpdate(meteors.get(), isDeadCameraShake);
	}
	//ボス敵の被ダメージ処理
	if (bEnemy->GetIsInit() == true)
	{
		if (isBEnemyDeadScene_ == true)
		{
			effect_->B_ParticleUpdate(bEnemy.get(), isDeadCameraShake);
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
			getline(line_stream, word, ' ');
			//敵の生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy->EnemyInitialize();
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
			newInvincibleEnemy->SetScale(Vector3(2.5f, 2.5f, 2.5f));
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
		player_->LaserOff();
		ui_->Stop();
		camera_->BossInitCameraWork(bEnemy.get(), isBossInitCamera_);
	}
	else
	{
		player_->LaserOn();
	}
}

void GamePlayScene::BossDead()
{
	//UI退場
	ui_->UIOutMotion();
	player_->LaserOff();
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
		}
	}
	else
	{
		deadCameraPos_ = camera_->GetViewProjection()->GetEye();
	}
}