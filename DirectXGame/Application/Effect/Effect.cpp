/**
* @file Effect.cpp
* @brief アプリ面のエフェクト処理クラス
* @author イグチ_シュンペイ
*/

#include "Effect.h"

void Effect::Initialize()
{
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	//パーティクル
	//ヒットテクスチャ
	p_Hit.reset(Particle::LoadParticleTexture("effect01.png"));
	pm_Hit = ParticleManager::Create();
	pm_Hit->SetParticleModel(p_Hit.get());
	pm_Hit->SetViewProjection(viewProjection_);
	//wEnemyテクスチャ
	p_WDmg.reset(Particle::LoadParticleTexture("effect02.png"));
	pm_WDmg = ParticleManager::Create();
	pm_WDmg->SetParticleModel(p_WDmg.get());
	pm_WDmg->SetViewProjection(viewProjection_);
	//bEnemyテクスチャ
	p_BDmg.reset(Particle::LoadParticleTexture("effect03.png"));
	pm_BDmg = ParticleManager::Create();
	pm_BDmg->SetParticleModel(p_BDmg.get());
	pm_BDmg->SetViewProjection(viewProjection_);
	//meteorテクスチャ
	p_Meteor.reset(Particle::LoadParticleTexture("effect04.png"));
	pm_Meteor = ParticleManager::Create();
	pm_Meteor->SetParticleModel(p_Meteor.get());
	pm_Meteor->SetViewProjection(viewProjection_);
	//基本爆発テクスチャ
	p_Ex.reset(Particle::LoadParticleTexture("effect05.png"));
	pm_Ex = ParticleManager::Create();
	pm_Ex->SetParticleModel(p_Ex.get());
	pm_Ex->SetViewProjection(viewProjection_);
	//自機爆発テクスチャ
	p_PEx.reset(Particle::LoadParticleTexture("effect06.png"));
	pm_PEx = ParticleManager::Create();
	pm_PEx->SetParticleModel(p_PEx.get());
	pm_PEx->SetViewProjection(viewProjection_);
	//自機爆発テクスチャ
	p_Smoke.reset(Particle::LoadParticleTexture("effect07.png"));
	pm_Smoke = ParticleManager::Create();
	pm_Smoke->SetParticleModel(p_Smoke.get());
	pm_Smoke->SetViewProjection(viewProjection_);

	//爆発
	explosion01_ = std::make_unique<Explosion>();
	explosion01_->ExplosionInitialize(0);
	explosion02_ = std::make_unique<Explosion>();
	explosion02_->ExplosionInitialize(1);
	explosion03_ = std::make_unique<Explosion>();
	explosion03_->ExplosionInitialize(2);
}

void Effect::Update()
{
	//パーティクル更新
	pm_Hit->Update();
	pm_WDmg->Update();
	pm_BDmg->Update();
	pm_Meteor->Update();
	pm_Ex->Update();
	pm_PEx->Update();
	pm_Smoke->Update();
}

void Effect::Draw(bool&isBEnemyDeadScene,bool& isPlayerDead)
{

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

	Object3d::PreDraw(dxCommon_->GetCommandList());

	if (isBEnemyDeadScene == true)
	{
		explosion01_->ExplosionDraw(viewProjection_);
		explosion02_->ExplosionDraw(viewProjection_);
	}

	if (isPlayerDead == true && explosion03_->GetIsFinish() == false)
	{
		explosion03_->ExplosionDraw(viewProjection_);
	}

	Object3d::PostDraw();
}

void Effect::H_ParticleUpdate(InvincibleEnemy* invincibleEnemy)
{
	if (invincibleEnemy->GetIsHit() == true)
	{
		pm_Hit->Fire(p_Hit.get(), 40, invincibleEnemy->GetPosition(), 5, false, { 1.0f, 0.0f });
	}
}

void Effect::E_ParticleUpdate(Enemy* enemy,bool& isDeadCameraShake)
{
	if (enemy->GetIsDead() == true)
	{
		isDeadCameraShake = true;
		pm_Meteor->Fire(p_Meteor.get(), 40, enemy->GetPosition(), 12, false, { 1.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 30, enemy->GetPosition(), 5, false, { 4.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 20, enemy->GetPosition(), 1, true, { 18.0f, 0.0f });
		pm_Smoke->Fire(p_Smoke.get(), 30, enemy->GetPosition(), 5, false, { 5.0f, 0.0f });
	}
}

void Effect::M_ParticleUpdate(Meteor* meteor, bool& isDeadCameraShake)
{
	if (meteor->GetIsDead() == true)
	{
		isDeadCameraShake = true;
		pm_Meteor->Fire(p_Meteor.get(), 40, meteor->GetPosition(), 12, false, { 1.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 30, meteor->GetPosition(), 5, false, { 4.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 20, meteor->GetPosition(), 1, true, { 18.0f, 0.0f });
		pm_Smoke->Fire(p_Smoke.get(), 30, meteor->GetPosition(), 5, false, { 5.0f, 0.0f });
	}
}

void Effect::W_ParticleUpdate(WeakEnemy* weakEnemy, bool& isDeadCameraShake)
{
	if (weakEnemy->GetIsDead() == true)
 	{
		isDeadCameraShake = true;
   		pm_WDmg->Fire(p_WDmg.get(), 40, weakEnemy->GetPosition(), 12, false, { 1.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 30, weakEnemy->GetPosition(), 5, false, { 4.0f, 0.0f });
		pm_Ex->Fire(p_Ex.get(), 20, weakEnemy->GetPosition(), 1, true, { 18.0f, 0.0f });
		pm_Smoke->Fire(p_Smoke.get(), 30, weakEnemy->GetPosition(), 5, false, { 5.0f, 0.0f });
	}
}

void Effect::B_ParticleUpdate(BossEnemy* bossEnemy, bool& isDeadCameraShake)
{
	if (bossEnemy->GetIsDead() == false)
	{
		isDeadCameraShake = true;
		explosion01_->SetPosition(bossEnemy->GetPosition());
		explosion02_->SetPosition(bossEnemy->GetPosition());
		pm_Ex->Fire(p_Ex.get(), 20, bossEnemy->GetPosition(), 4, false, { 8.0f, 0.0f });
	}
}

void Effect::Ex_ParticleUpdate(Player* player, bool& isDead)
{
	if (isDead == false)
	{
		pm_Ex->PlayerExBefore(p_Ex.get(), 20, player->GetPosition(), 2, { 2.0f, 0.0f });
		//自機を動かす
		player_->worldTransform_.rotation_.z += 8.0f;
		if (player_->worldTransform_.rotation_.z >= 720.0f)
		{
			isDead = true;
			pm_PEx->PlayerExAfter(p_PEx.get(), 200, player->GetPosition(), 12, { 1.0f, 0.0f });
		}
	}
}
