/**
* @file ShotEnemy.cpp
* @brief 弾を発射する敵の処理クラス
* @author イグチ_シュンペイ
*/

#include "ShotEnemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"
#include "Vector3.h"

//デストラクタ
ShotEnemy::~ShotEnemy()
{

}

//初期化
void ShotEnemy::ShotEnemyInitialize()
{
	//汎用的初期化
	EnemyInitialize();
	// OBJからモデルデータを読み込む
	enemyModel_01 = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel_01.get());

	bulletColliderPos_ = { 0.0f,0.0f,0.0f };
	bulletColliderRadius_ = 0.5f;

	kBulletSpeed_ = 1.0f;

	dalayTimer_ = 100.0f;
	delayTImerMax_ = 15.0f;
	isDead_ = false;
	isDelete_ = false;
	isHit_ = false;
	isInit_ = false;
	initTime_ = 0.0f;
	initTimeMax_ = 180.0f;
	initY_ = 60.0f;
	afterInitY_ = 60.0f;
	bulletNum_ = 0;
	bulletMax_ = 3;
	moveLen_ = 30.0f;
	moveTime_ = 0.0f;
	moveTimeMax_ = 345.0f;
}

void ShotEnemy::ShotEnemyUpdate(Vector3 playerPos)
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	//登場モーション
	InitMotion();
	if (isInit_ == true)
	{
		Move();
		//攻撃したら退場
		if (isAttack_ == false)
		{
			Attack(playerPos);
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

	//敵汎用更新
	EnemyUpdate();
}

void ShotEnemy::Move()
{
	if (isBack_ == false)
	{
		if (moveTime_ <= moveTimeMax_)
		{
			//csvファイルから読み取った方向に移動
			switch (moveNum_)
			{
			case MoveNum::Up:   //カーブフェーズ
				worldTransform_.position_.y = beforeMovePos_.y + (moveLen_ * MathFunc::easeOutSine(moveTime_ / moveTimeMax_));
				break;
			case MoveNum::Down:   //カーブフェーズ
				worldTransform_.position_.y = beforeMovePos_.y - (moveLen_ * MathFunc::easeOutSine(moveTime_ / moveTimeMax_));
				break;
			case MoveNum::Right:   //カーブフェーズ
				worldTransform_.position_.x = beforeMovePos_.x + (moveLen_ * MathFunc::easeOutSine(moveTime_ / moveTimeMax_));
				break;
			case MoveNum::Left:   //カーブフェーズ
				worldTransform_.position_.x = beforeMovePos_.x - (moveLen_ * MathFunc::easeOutSine(moveTime_ / moveTimeMax_));
				break;
			}
			moveTime_++;
		}
		else
		{
			//移動しきったら退場モーションに使うy座標を更新
			beforeY_ = worldTransform_.position_.y;
			isBack_ = true;
		}
	}
}

void ShotEnemy::Attack(Vector3 playerPos_)
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
		newBullet->SetCollider(new SphereCollider(bulletColliderPos_, bulletColliderRadius_));

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

void ShotEnemy::InitMotion()
{
	if (isInit_ == false)
	{
		worldTransform_.position_.y = beforeY_ + (afterInitY_ - (initY_ * MathFunc::easeOutBack(initTime_ / initTimeMax_)));
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isInit_ = true;
			initTime_ = 0.0f;
			beforeMovePos_ = worldTransform_.position_;
		}
	}
}

void ShotEnemy::BackMotion()
{
	if (isBack_ == true)
	{
		worldTransform_.position_.y = beforeY_ + initY_ * MathFunc::easeInBack(initTime_ / initTimeMax_);
		initTime_++;
		if (initTime_ > initTimeMax_)
		{
			isBack_ = true;
			isDelete_ = true;
		}
	}
}

void ShotEnemy::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_, 1.0f, bullet->GetColor());
	}
}