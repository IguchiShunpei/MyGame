/**
* @file BossEnemy.h
* @brief ボス敵の処理クラス
* @author イグチ_シュンペイ
*/

#include "BossEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
BossEnemy::~BossEnemy()
{
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
	SetModel(enemyModel.get());
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 3.0f));
	SetScale({ 2.0f,2.0f,2.0f });

	//登場
	beforeY_ = 120.0f;
	SetPosition({ 0.0f,beforeY_,100.0f });
	isInit_ = false;
	initY_ = beforeY_;
	afterInitY_ = initY_;
	initTime_ = 0.0f;
	initTimeMax_ = 240.0f;

	//死亡
	isDead_ = false;
	isDeathTimer_ = false;
	deathTimer_ = 180;

	//弾攻撃
	dalayTimer_ = 5.0f;

	//回転攻撃
	turnTimer_ = 0.0f;
	turnTimerMax_ = 120.0f;
	turnSpeed_ = 0.0f;
	turnSpeedMax_ = 30.0f;
	turnAttackTimer_ = 0.0f;
	turnAttackTimerMax_ = 45.0f;
	playerPos_ = { 0.0f,0.0f,0.0f };
	backRotaY_ = 0.0f;

	//待機
	waitTimer_ = 0.0f;
	waitTimerMax_ = 300.0f;
	waitMotionTimer_ = 0.0f;
	waitMotionTimerMax_ = 150.0f;
	waitMotionRange_ = 1.0f;
	isUp_ = false;

	//離脱
	leaveTimer_ = 0.0f;
	leaveTimerMax_ = 120.0f;

	phase_ = Phase::Wait;

	bossColor_ = { 1.0f,1.0f,1.0f };
	hitColor_ = { 3.0f,3.0f,3.0f };

	hp_ = 30;

	//シェイク範囲
	shakeRange_ = 3.0f;
	//シェイク移動量
	shakeNum_ = 0.1f;
	//ボスの墜落スピード
	bossDownSpeed_ = 0.01f;
	//ボスalpha
	bossAlpha_ = 1.0f;
	bossAlphaMax_ = 1.0f;
	bossAlphaMin_ = 0.5f;
	//ボスalphaに代入する数
	bossAlphaNum_ = 0.05f;
	bossScaleNum_ = { 0.01f,0.01f,0.01f };
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

	//弾更新
	BulletUpdate();

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void BossEnemy::Dead()
{
	//墜落
	if (worldTransform_.position_.x > 0.0f)
	{
		worldTransform_.position_.y -= bossDownSpeed_;
	}

	worldTransform_.scale_ -= bossScaleNum_;

	//左右にシェイク
	if (isShake_ == true)
	{
		if (worldTransform_.position_.x <= shakeRange_)
		{
			worldTransform_.position_.x += shakeNum_;
		}
		else
		{
			isShake_ = false;
		}
	}
	else
	{
		if (worldTransform_.position_.x >= -shakeRange_)
		{
			worldTransform_.position_.x -= shakeNum_;
		}
		else
		{
			isShake_ = true;
		}
	}
	//alpha値を変化
	if (isAlpha_ == true)
	{
		if (alpha_ <= bossAlphaMax_)
		{
			alpha_ += bossAlphaNum_;
		}
		else
		{
			isAlpha_ = false;
		}
	}
	else
	{
		if (alpha_ >= bossAlphaMin_)
		{
			alpha_ -= bossAlphaNum_;
		}
		else
		{
			isAlpha_ = true;
		}
	}

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
		if (isInit_ == true && isTurn_ == false)
		{
			isHit_ = true;
			Damage(damage_);
		}
	}
}

void BossEnemy::Wait()
{
	WaitMotion();
	if (waitTimer_ < waitTimerMax_)
	{
		waitTimer_++;
	}
	else
	{
		waitTimer_ = 0.0f;
		phase_ = Phase::Turn;
	}
}

void BossEnemy::WaitMotion()
{
	//上昇
	if (isUp_ == true)
	{
		if (waitMotionTimer_ < waitMotionTimerMax_)
		{
			worldTransform_.position_.y = beforeY_ + waitMotionRange_ * MathFunc::easeOutSine(waitMotionTimer_ / waitMotionTimerMax_);
			waitMotionTimer_++;
		}
		else
		{
			isUp_ = false;
			waitMotionTimer_ = 0;
			beforeY_ = worldTransform_.position_.y;
		}
	}
	//下降
	else
	{
		if (waitMotionTimer_ < waitMotionTimerMax_)
		{
			worldTransform_.position_.y = beforeY_ + waitMotionRange_ * -MathFunc::easeOutSine(waitMotionTimer_ / waitMotionTimerMax_);
			waitMotionTimer_++;
		}
		else
		{
			isUp_ = true;
			waitMotionTimer_ = 0;
			beforeY_ = worldTransform_.position_.y;
		}
	}
}

void BossEnemy::Move()
{

}

void BossEnemy::PhaseChange(Vector3 playerPos)
{
	if (isDeathTimer_ == false)
	{
		switch (phase_)
		{
		case Phase::Wait: //待機フェーズ
			//待機
			Wait();
			break;
		case Phase::Bullet://弾攻撃フェーズ
			Move();
			BulletAttack();
			break;
		case Phase::Turn:   //回転攻撃フェーズ
			TurnAttack(playerPos);
			break;
		case Phase::Leave:   //離脱
			Leave();
			break;
		}
	}
	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void BossEnemy::BulletAttack()
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

void BossEnemy::TurnAttack(Vector3 pos)
{
	isTurn_ = true;
	//回転
	worldTransform_.rotation_.y += turnSpeed_;
	if (isApproach_ == false)
	{
		//予備動作タイマー
		if (turnTimer_ < turnTimerMax_)
		{
			//回転タイマー
			turnTimer_++;
			if (turnSpeed_ < turnSpeedMax_)
			{
				//回転速度を加速
				turnSpeed_++;
			}
			else
			{
				//最高速度で継続
				turnSpeed_ = turnSpeedMax_;
			}
		}
		else
		{
			//接近する
			isApproach_ = true;
			turnTimer_ = 0.0f;
			playerPos_ = pos;
		}
	}
	else
	{
		//攻撃タイマー
		if (turnAttackTimer_ < turnAttackTimerMax_)
		{
			//自機の位置に接近
			worldTransform_.position_.x = turnAttackPos_.x + (turnAttackPos_.x - playerPos_.x) * -MathFunc::easeOutSine(turnAttackTimer_ / turnAttackTimerMax_);
			worldTransform_.position_.y = turnAttackPos_.y + (turnAttackPos_.y - playerPos_.y) * -MathFunc::easeOutSine(turnAttackTimer_ / turnAttackTimerMax_);
			worldTransform_.position_.z = turnAttackPos_.z + (turnAttackPos_.z - playerPos_.z + 3.0f) * -MathFunc::easeOutSine(turnAttackTimer_ / turnAttackTimerMax_);
			turnAttackTimer_++;
		}
		else
		{
			isApproach_ = false;
			turnAttackTimer_ = 0.0f;
			phase_ = Phase::Leave;
			beforeLeavePos_ = GetPosition();
			backRotaY_ = worldTransform_.rotation_.y;
		}
	}
}

void BossEnemy::InitMotion()
{

	if (isInit_ == false)
	{
		beforeY_ = 0.0f;
		worldTransform_.position_.y = beforeY_ + (afterInitY_ - (initY_ * MathFunc::easeOutBack(initTime_ / initTimeMax_)));
		worldTransform_.UpdateMatrix();
		initTime_++;
		if (initTime_ >=initTimeMax_)
		{
			isInit_ = true;
			//初期位置をセット
			normalPos_ = worldTransform_.position_;
			turnAttackPos_ = GetPosition();
			beforeY_ = worldTransform_.position_.y;
		}
	}
}

void BossEnemy::Damage(int damage)
{
	bossColor_ = hitColor_;
	hp_ -= damage;
	if (hp_ <= 0)
	{
		isDeathTimer_ = true;
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

void BossEnemy::Leave()
{
	if (leaveTimer_ < leaveTimerMax_)
	{
		if (isTurn_ == true)
		{
			//回転し分を戻す
			worldTransform_.rotation_.y = backRotaY_ - backRotaY_ * MathFunc::easeOutSine(leaveTimer_ / leaveTimerMax_);
		}
		//原点に戻る
		worldTransform_.position_.x = beforeLeavePos_.x + (turnAttackPos_.x - beforeLeavePos_.x) * MathFunc::easeOutSine(leaveTimer_ / leaveTimerMax_);
		worldTransform_.position_.y = beforeLeavePos_.y + (turnAttackPos_.y - beforeLeavePos_.y) * MathFunc::easeOutSine(leaveTimer_ / leaveTimerMax_);
		worldTransform_.position_.z = beforeLeavePos_.z + (turnAttackPos_.z - beforeLeavePos_.z - 3.0f) * MathFunc::easeOutSine(leaveTimer_ / leaveTimerMax_);
		leaveTimer_++;
	}
	else
	{
		leaveTimer_ = 0.0f;
		isTurn_ = false;
		phase_ = Phase::Wait;
	}
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
