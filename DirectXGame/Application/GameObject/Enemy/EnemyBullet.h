/**
* @file EnemyBullet.cpp
* @brief 敵の弾の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once
#include "Object3d.h"
#include "Model.h"
#include <memory>

class EnemyBullet : public Object3d
{
public:
	//デストラクタ
	~EnemyBullet();
	//初期化
	void EnemyBulletInitialize(const Vector3& position, const Vector3& velocity);
	//当たり判定更新
	void ColliderUpdate();

	void Update();

	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//isDeadのgetter
	bool GetIsDelete() const { return isDelete_; }

private:

	// モデル
	std::unique_ptr<Model> enemyBulletModel_;
	//削除フラグ
	bool isDelete_;
	//削除タイマー
	float deleteTimer_;
	//速度
	Vector3 velocity_;
};