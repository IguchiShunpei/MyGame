/**
 * @file EnemyManager.h
 * @brief 敵全体のクラス
 * @author イグチ_シュンペイ
 */

#pragma once

#include "ShotEnemy.h"
#include "InvincibleEnemy.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"
#include "Item.h"
#include "Meteor.h"
#include "Player.h"
#include "Effect.h"
#include "Camera.h"

#include<string>
#include <sstream>

class EnemyManager
{
public:
	//デストラクタ
	~EnemyManager();


	//敵リスト
	const std::list<std::unique_ptr<ShotEnemy>>& GetEnemys() { return enemys_; }
	const std::list<std::unique_ptr<WeakEnemy>>& GetWeakEnemys() { return wEnemys_; }
	const std::list<std::unique_ptr<InvincibleEnemy>>& GetInvEnemys() { return invincibleEnemys_; }
	const std::list<std::unique_ptr<Meteor>>& GetMeteors() { return meteors_; }

	//ボス
	BossEnemy* GetBossEnemy() const { return bEnemy_.get(); }

	//アイテムリスト
	const std::list<std::unique_ptr<Item>>& GetItems() { return items_; }

private:
	//敵
	std::unique_ptr<ShotEnemy> enemy_;
	std::unique_ptr<WeakEnemy> wEnemy_;
	std::unique_ptr<InvincibleEnemy> invincibleEnemy_;
	std::unique_ptr<BossEnemy> bEnemy_;

	//敵
	std::list<std::unique_ptr<ShotEnemy>> enemys_;
	std::list<std::unique_ptr<WeakEnemy>> wEnemys_;
	std::list<std::unique_ptr<InvincibleEnemy>> invincibleEnemys_;

	//アイテムを持っている敵
	std::unique_ptr < Meteor> meteor_;
	std::list<std::unique_ptr<Meteor>> meteors_;
	//アイテム
	std::list<std::unique_ptr<Item>> items_;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;

	//敵を発生させるときの待機フラグ
	bool isWait_;

	//タイマー
	//敵を発生させるときの待機時間
	int waitTimer_;
};

