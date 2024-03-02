/**
* @file Enemy.cpp
* @brief 弾を発射する敵の処理クラス
* @author イグチ_シュンペイ
*/

#include "Enemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"
#include "Vector3.h"

//デストラクタ
Enemy::~Enemy()
{

}

//初期化
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	enemyModel_01 = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel_01.get());

	bulletColliderPos_ = {0.0f,0.0f,0.0f};
	bulletColliderRadius_ = 0.5f;

	kBulletSpeed_ = 2.0f;

	dalayTimer_ = 100.0f;
	delayTImerMax_ = 15.0f;
	deleteTimer_ = 200.0f;
	isDead_ = false;
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 180.0f;
	moveY_ = 60.0f;
	afterMoveY_ = 60.0f;
	bulletNum_ = 0;
	bulletMax_ = 3;
	enemyColor_ = { 0.0f,0.0f,0.0f };
	originalColor_ = { 1.0f,1.0f,1.0f };
	changeColor_ = { 3.0f,3.0f,3.0f };
}

void Enemy::Update(Vector3 playerPos)
{
	isHit_ = false;
	enemyColor_ = originalColor_;
	changeColor_ = { 3.0f,3.0f,3.0f };

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	//登場モーション
	InitMotion();
	if (isInit_ == true)
	{
		//攻撃したら退場
		if (isAttack_ == false)
		{
			Attack(playerPos);
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

	//退場モーション
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

void Enemy::Damage(int damage)
{
	//攻撃を受けたときに変色
	enemyColor_ = changeColor_;
 	//ダメージ処理
  	hp_ -= damage;
	if (hp_ <= 0)
	{
		isDead_ = true;
	}
}

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
  {
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
 	{
		isHit_ = true;
 		Damage(damage_);
	}
}

void Enemy::Attack(Vector3 playerPos_)
{
	dalayTimer_--;

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = playerPos_;
	//自キャラの座標をコピー
	Vector3 enemyPosition = GetPosition();

	//自機の座標との差分ベクトルを求める
	velocity_ = enemyPosition - playerPosition;

	//長さを求める
	velocity_.length();
	//正規化
	velocity_.normalize();
	//ベクトルの長さを,速さに合わせる
	velocity_ *= kBulletSpeed_;//これが速度になる

	//クールタイムが０になったとき
	if (dalayTimer_ <= 0)
	{
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->EnemyBulletInitialize(enemyPosition, velocity_);

		//コライダーの追加
		newBullet->SetCollider(new SphereCollider(bulletColliderPos_,bulletColliderRadius_));

		//球の登録
		bullets_.push_back(std::move(newBullet));

		dalayTimer_ = delayTImerMax_;
		bulletNum_++;
		//最大数発射するまで繰り返す
		if (bulletNum_ > bulletMax_)
		{
			isAttack_ = true;
		}
	}
}

void Enemy::InitMotion()
{
	if (isInit_ == false)
	{
 		worldTransform_.position_.y = beforeY_ + (afterMoveY_ - (moveY_ * MathFunc::easeInOutBack(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
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
		worldTransform_.position_.y = beforeY_ + moveY_ * MathFunc::easeInOutBack(initTime_ / initTimeMax_);
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
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
