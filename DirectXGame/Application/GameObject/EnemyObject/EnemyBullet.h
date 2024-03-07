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
	//色
	Vector3 GetColor() const { return color_; }

private:

	// モデル
	std::unique_ptr<Model> enemyBulletModel_;
	//削除フラグ
	bool isDelete_;
	//削除タイマー
	float deleteTimer_;
	//速度
	Vector3 velocity_;
	//色
	Vector3 color_;
	//回転量
	float rota_;
};