#pragma once
#include "Object3d.h"
#include "Model.h"

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
	Model* enemyBulletModel_ = nullptr;
	//削除フラグ
	bool isDelete_ = false;
	//削除タイマー
	float deleteTimer_ = 200.0f;
	//速度
	Vector3 velocity_;
};