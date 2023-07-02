#pragma once
#include "Player.h"
#include "Object3d.h"
#include "Model.h"

class PlayerBullet : public Object3d
{
public:
	//デストラクタ
	~PlayerBullet();
	//初期化
	void PlayerBulletInitialize(const Vector3& position, const Vector3& velocity);

	void Update();

	void OnCollision(const CollisionInfo& info) override;

	//isDeadのgetter
	bool IsDelete() const { return isDelete_; }

private:

	// モデル
	Model* playerBulletModel_ = nullptr;
	//削除フラグ
	bool isDelete_ = false;
	//削除タイマー
	float deleteTimer_ = 60.0f;
	//速度
	Vector3 velocity_;
};