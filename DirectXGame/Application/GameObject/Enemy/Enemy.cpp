#include "Enemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
Enemy::~Enemy()
{
	delete enemyModel;
	delete enemyBullet;
}

//初期化
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel = Model::LoadFromOBJ("enemy01");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);
	dalayTimer_ = 5.0f;
	deleteTimer_ = 200.0f;
	isDead_ = false;
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 60.0f;
}

void Enemy::Update()
{
	isHit_ = false;
	enemyColor_ = { 1.0f,1.0f,1.0f };

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	InitMotion();
	if (isInit_ == true)
	{
		if (isAttack_ == false)
		{
		Attack();
		}
		else
		{
			deleteTimer_--;
			if (deleteTimer_ <= 0.0f)
			{
				isBack_ = true;
			}
		}
	}

	BackMotion();

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

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{

		isHit_ = true;
		enemyColor_ = { 3.0f,3.0f,3.0f };
		hp_--;
		if (hp_ <= 0)
		{
			isDead_ = true;
		}
	}
}

void Enemy::Attack()
{
	dalayTimer_ -= 0.1f;

	//自キャラの座標をコピー
	Vector3 position = GetPosition();

	//弾の速度
	const float kBulletSpeed = 1.5f;
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

		isAttack_ = true;
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
	case Phase::Curve:   //カーブフェーズ
		Curve();
		break;
	case Phase::ReCurve:   //カーブフェーズ
		ReCurve();
		break;
	}
}

void Enemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + 60.0f * MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_--;
		if (initTime_ <= 0.0f)
		{
			isInit_ = true;
			initTime_ = 0.0f;
		}
	}
}

void Enemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ + 120.0f * MathFunc::easeInSine(initTime_ / 60.0f);
		initTime_++;
		if (initTime_ >= 60.0f)
		{
			isBack_ = true;
			isDelete_ = true;
		}
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

	//既定の位置に着いたらカーブへ
	if (worldTransform_.position_.z >= 100.0f)
	{
		phase_ = Phase::Approach;
	}
}

void Enemy::Curve()
{
	MathFunc::CurveProjection(worldTransform_, startSpeed, C, flame);
}

void Enemy::ReCurve()
{
	MathFunc::CurveProjection(worldTransform_, { -startSpeed.x,startSpeed.y,startSpeed.z }, -C, flame);
}

void Enemy::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Enemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
