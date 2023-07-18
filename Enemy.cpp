#include "Enemy.h"
#include "SphereCollider.h"
#include "GameScene.h"
#include "string.h"

//デストラクタ
Enemy::~Enemy() 
{
	delete enemyModel;
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		delete enemyBullet;
	}
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
	isDead_ = false;
}

void Enemy::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	Move();

	Attack();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void Enemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0) 
	{
		isDead_ = true;
	}
}

void Enemy::Attack()
{
	dalayTimer -= 0.1f;

	//自キャラの座標をコピー
	Vector3 position = GetWorldPosition();

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//クールタイムが０になったとき
	if (dalayTimer <= 0)
	{
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->EnemyBulletInitialize(position, velocity);

		//コライダーの追加
		newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

		//球の登録
		bullets_.push_back(std::move(newBullet));

		dalayTimer = 5.0f;
	}
}

void Enemy::Move()
{
	switch (phase_)
	{
	case Phase::Approach: //接近フェーズ
	default:
		//移動(ベクトルを加算)
		Approach();
		break;
	case Phase::Leave:   //離脱フェーズ
		Leave();
		break;

	}
}

void Enemy::Approach()
{
	worldTransform_.position_ += approach_;

	//既定の位置に着いたら離脱へ
	if (worldTransform_.position_.z <= 30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	worldTransform_.position_ += leave_;

	//既定の位置に着いたら接近へ
	if (worldTransform_.position_.z >= 80.0f)
	{
		phase_ = Phase::Approach;
	}
}

void Enemy::Curve()
{

}

void Enemy::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
