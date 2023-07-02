#include "Enemy.h"
#include "SphereCollider.h"
#include "GameScene.h"
#include "string.h"

//デストラクタ
Enemy::~Enemy() {
	delete enemyModel;
}

//初期化
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);
	SetPosition(Vector3(0, 0, 100));
	isDead_ = false;
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0) {
		isDead_ = true;
	}
}