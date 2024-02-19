/**
* @file Effect.h
* @brief アプリ面のエフェクト処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "ParticleManager.h"
#include "Explosion.h"
#include "DirectXCommon.h"
#include "Player.h"
#include "Meteor.h"
#include "Enemy.h"
#include "InvincibleEnemy.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"

class Effect
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(bool& isBEnemyDeadScene, bool& isPlayerDead);

	//各オブジェクトの処理
	void H_ParticleUpdate(InvincibleEnemy* invincibleEnemy);
	void E_ParticleUpdate(Enemy* enemy, bool& isDeadCameraShake_);
	void M_ParticleUpdate(Meteor* meteor, bool& isDeadCameraShake_);
	void W_ParticleUpdate(WeakEnemy* weakEnemy, bool& isDeadCameraShake_);
	void B_ParticleUpdate(BossEnemy* bossEnemy, bool& isDeadCameraShake_);
	void Ex_ParticleUpdate(Player* player,bool& isDead);

	//getter
	Explosion* GetExplosion01() const { return explosion01_.get(); };
	Explosion* GetExplosion02() const { return explosion02_.get(); };
	Explosion* GetExplosion03() const { return explosion03_.get(); };

	//setter
	void SetView(ViewProjection* viewProjection) {viewProjection_ = viewProjection; }
	void SetPlayer(Player* player) { player_ = player; }

private:
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_ = nullptr;

	//viewProjection
	ViewProjection* viewProjection_ = nullptr;

	//player
	Player* player_ = nullptr;

	//パーティクル
	//弾が当たったエフェクト
	std::unique_ptr < Particle>p_Hit;
	std::unique_ptr < ParticleManager> pm_Hit;
	//雑魚敵が死亡したときのエフェクト
	std::unique_ptr < Particle> p_WDmg;
	std::unique_ptr < ParticleManager> pm_WDmg;
	//ボスが死亡したときのエフェクト
	std::unique_ptr < Particle> p_BDmg;
	std::unique_ptr < ParticleManager> pm_BDmg;
	//隕石を壊した時のエフェクト
	std::unique_ptr < Particle>p_Meteor;
	std::unique_ptr < ParticleManager> pm_Meteor;
	//全般的な爆発エフェクト
	std::unique_ptr < Particle> p_Ex;
	std::unique_ptr < ParticleManager> pm_Ex;
	//自機の死亡時エフェクト
	std::unique_ptr < Particle> p_PEx;
	std::unique_ptr < ParticleManager> pm_PEx;
	//煙
	std::unique_ptr < Particle> p_Smoke;
	std::unique_ptr < ParticleManager> pm_Smoke;

	//爆発
	std::unique_ptr <Explosion> explosion01_;
	std::unique_ptr <Explosion> explosion02_;
	std::unique_ptr <Explosion> explosion03_;
};