#include "BossEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
BossEnemy::~BossEnemy()
{
	delete enemyModel;
}

//初期化
void BossEnemy::BossEnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy04");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
	SetScale({ 1.0f,1.0f,1.0f });
	beforeY_ = 120.0f;
	SetPosition({ 0.0f,beforeY_,100.0f });
	isDead_ = false;
	isDeathTimer_ = false;
	deathTimer_ = 180;
	isInit_ = false;
	initTime_ = 240.0f;
	hp_ = 50;
	dalayTimer_ = 5.0f;
}

void BossEnemy::Update()
{
	isHit_ = false;

	bossColor_ = { 1.0f,1.0f,1.0f };

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	//死亡タイマーの処理
	ActiveDeathTimer();
	//移動処理
	Move();

	Attack();

	BulletUpdate();

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void BossEnemy::BulletUpdate()
{
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}
}

void BossEnemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void BossEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		if (isInit_ == true)
		{
			isHit_ = true;
			bossColor_ = { 3.0f,3.0f,3.0f };
			hp_--;
			if (hp_ == 0)
			{
				isDeathTimer_ = true;
			}
		}
	}
}

void BossEnemy::Move()
{
	if (isDeathTimer_ == false)
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
		case Phase::Curve:   //カーブフェーズ
			Curve();
			break;
		case Phase::ReCurve:   //カーブフェーズ
			ReCurve();
			break;
		}
	}
}

void BossEnemy::Attack()
{
	dalayTimer_ -= 0.1f;

	//自キャラの座標をコピー
	Vector3 position = GetPosition();

	//弾の速度
	const float kBulletSpeed = 0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//クールタイムが０になったとき
	if (dalayTimer_ <= 0)
	{
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->EnemyBulletInitialize(position, velocity);

		//コライダーの追加
		newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

		//球の登録
		bullets_.push_back(std::move(newBullet));

		dalayTimer_ = 5.0f;
	}
}

void BossEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ * MathFunc::easeInSine(initTime_ / 240.0f);
		worldTransform_.UpdateMatrix();
		initTime_--;
		if (initTime_ <= 0.0f)
		{
			isInit_ = true;
		}
	}
}

void BossEnemy::ActiveDeathTimer()
{
	isHit_ = false;
	if (isDeathTimer_ == true)
	{
		if (deathTimer_ <= 0)
		{
			isDead_ = true;
		}
		deathTimer_--;
	}
}

void BossEnemy::Approach()
{
	worldTransform_.position_ += approach_;

	//既定の位置に着いたら離脱へ
	if (worldTransform_.position_.z <= 30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void BossEnemy::Leave()
{
	worldTransform_.position_ += leave_;

	//既定の位置に着いたらカーブへ
	if (worldTransform_.position_.z >= 100.0f)
	{
		phase_ = Phase::Approach;
	}
}

void BossEnemy::Curve()
{
	MathFunc::CurveProjection(worldTransform_, startSpeed, C, flame);
}

void BossEnemy::ReCurve()
{
	MathFunc::CurveProjection(worldTransform_, { -startSpeed.x,startSpeed.y,startSpeed.z }, -C, flame);
}

void BossEnemy::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}


Vector3 BossEnemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
