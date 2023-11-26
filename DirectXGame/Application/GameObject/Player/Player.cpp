#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GamePlayScene.h"

//デストラクタ
Player::~Player()
{
	delete playerModel_;
	delete playerBullet_;
}

//初期化
void Player::PlayerInitialize()
{
	Initialize();

	// OBJからモデルデータを読み込む
	playerModel_ = Model::LoadFromOBJ("fighter");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerModel_);
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
	SetPosition(Vector3(0, -2, -20));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	//変数の初期化
	speed_ = { 0.0f,0.0f,0.0f };
	speedU_ = 0.0f;
	speedD_ = 0.0f;
	speedR_ = 0.0f;
	speedL_ = 0.0f;
	speedMax_ = 0.2f;
	speedChange_ = 0.01f;
	bulletNum_ = 0;
	initMotionTime_ = 0.0f;
	dalayTimer_ = 0.0f;
	hp_ = 1;
	initSpeedZ_ = 0.5f;
	initRotaZ_ = 400.0f;
	initMotionTimeMax_ = 40.0f;

	//フラグ
	isMoveUp_ = false;
	isMoveDown_ = false;
	isMoveRight_ = false;
	isMoveLeft_ = false;
	isBurst_ = false;
	isInit_ = false;
	isHit_ = false;
	isDead_ = false;
	isInv_ = false;
	isChangeDir_ = false;
}

void Player::Update()
{
	input_ = Input::GetInstance();

	if (isDead_ == false)
	{
		Move();

		Rotate();

		//ChangeBullet();

		Attack();
	}

	BulletUpdate();

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void Player::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Player::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	const char* str1 = "class EnemyBullet";
	const char* str2 = "class InvEnemy";

	//相手がEnemy
	if (isInv_ == false)
	{
		if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0)
		{
			isHit_ = true;
		}
	}
	Damage();
}

void Player::IntitMotion()
{
	if (isInit_ == false && isInitAfter_ == false)
	{
		//前進
		worldTransform_.position_.z += initSpeedZ_;
		//イージングを使った回転
		worldTransform_.rotation_.z = initRotaZ_ * -MathFunc::easeOutSine(initMotionTime_ / initMotionTimeMax_);
		initMotionTime_++;
		//初期座標に来たら固定
		if (worldTransform_.position_.z >= 0.0f)
		{
			SetPosition(Vector3(0.0f, -2.0f, 0.0f));
			isInitAfter_ = true;
			initMotionTime_ = 10.0f;
		}
	}
	//余分に回転した分を戻す処理
	if (isInitAfter_ == true)
	{
		worldTransform_.rotation_.z += 4.0f;
		if (worldTransform_.rotation_.z >= -360.0f)
		{
			SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			isInitAfter_ = false;
			isInit_ = true;
		}
	}
	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void Player::Move()
{
	//上キーを押したとき
	if (input_->PushKey(DIK_UP))
	{
		//向きを変化
		isMoveUp_ = true;
	}
	else
	{
		isMoveUp_ = false;
	}
	//下キーを押したとき
	if (input_->PushKey(DIK_DOWN))
	{
		//向きを変化
		isMoveDown_ = true;
	}
	else
	{
		isMoveDown_ = false;
	}
	//右キーを押したとき
	if (input_->PushKey(DIK_RIGHT))
	{
		//向きを変化
		isMoveRight_ = true;
	}
	else
	{
		isMoveRight_ = false;
	}
	//左キーを押したとき
	if (input_->PushKey(DIK_LEFT))
	{
		//向きを変化
		isMoveLeft_ = true;
	}
	else
	{
		isMoveLeft_ = false;
	}

	SpeedChange();

	//移動限界座標
	const float kMoveLimitX = 9.0f * 1.7f;
	const float kMoveLimitY = 6.0f * 1.7f;

	//範囲を超えない処理
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, +kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitY);
	worldTransform_.position_.y = min(worldTransform_.position_.y, +kMoveLimitY);
}

void Player::SpeedChange()
{
	//キーを押しているときに加速
	//上
	if (isMoveUp_ == true)
	{
		if (speedU_ <= speedMax_)
		{
			speedU_ += speedChange_;
		}
		else
		{
			speedU_ = speedMax_;
		}
	}
	else
	{
		if (speedU_ >= 0.0f)
		{
			speedU_ -= speedChange_;
		}
		else
		{
			speedU_ = 0.0f;
		}
	}
	//下
	if (isMoveDown_ == true)
	{
		if (speedD_ >= -speedMax_)
		{
			speedD_ -= speedChange_;
		}
		else
		{
			speedD_ = -speedMax_;
		}
	}
	else
	{
		if (speedD_ <= 0.0f)
		{
			speedD_ += speedChange_;
		}
		else
		{
			speedD_ = 0.0f;
		}
	}
	if (isChangeDir_ == false)
	{
		//右
		if (isMoveRight_ == true)
		{
			if (speedR_ <= speedMax_)
			{
				speedR_ += speedChange_;
			}
			else
			{
				speedR_ = speedMax_;
			}
		}
		else
		{
			if (speedR_ >= 0.0f)
			{
				speedR_ -= speedChange_;
			}
			else
			{
				speedR_ = 0.0f;
			}
		}
		//左
		if (isMoveLeft_ == true)
		{
			if (speedL_ >= -speedMax_)
			{
				speedL_ -= speedChange_;
			}
			else
			{
				speedL_ = -speedMax_;
			}
		}
		else
		{
			if (speedL_ <= 0.0f)
			{
				speedL_ += speedChange_;
			}
			else
			{
				speedL_ = 0.0f;
			}
		}
	}
	else
	{
		//右
		if (isMoveRight_ == true)
		{
			if (speedL_ >= -speedMax_)
			{
				speedL_ -= speedChange_;
			}
			else
			{
				speedL_ = -speedMax_;
			}
		}
		else
		{
			if (speedL_ <= 0.0f)
			{
				speedL_ += speedChange_;
			}
			else
			{
				speedL_ = 0.0f;
			}
		}
		//左
		if (isMoveLeft_ == true)
		{
			if (speedR_ <= speedMax_)
			{
				speedR_ += speedChange_;
			}
			else
			{
				speedR_ = speedMax_;
			}
		}
		else
		{
			if (speedR_ >= 0.0f)
			{
				speedR_ -= speedChange_;
			}
			else
			{
				speedR_ = 0.0f;
			}
		}
	}


	speed_.x = speedR_ + speedL_;
	speed_.y = speedU_ + speedD_;

	//移動
	SetPosition(GetPosition() + speed_);
}

void Player::Rotate()
{
	//上キーを押したときの傾き処理
	if (isMoveUp_ == true)
	{
		if (worldTransform_.rotation_.x > -30.0f)
		{
			SetRotation(GetRotation() - Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x < 0.0f)
		{
			SetRotation(GetRotation() + Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	//下キーを押したときの傾き処理
	if (isMoveDown_ == true)
	{
		if (worldTransform_.rotation_.x < 30.0f)
		{
			SetRotation(GetRotation() + Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x > 0.0f)
		{
			SetRotation(GetRotation() - Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	if (isChangeDir_ == false)
	{
		//右キーを押したとき
		if (isMoveRight_ == true)
		{
			if (worldTransform_.rotation_.z > -30.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z < 0.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		//左キーを押したとき
		if (isMoveLeft_ == true)
		{
			if (worldTransform_.rotation_.z < 30.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z > 0.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -1.0f, 1.0f));
			}
		}
	}
	else
	{
		//右キーを押したとき
		if (isMoveRight_ == true)
		{
			if (worldTransform_.rotation_.z < 30.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z > 0.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		//左キーを押したとき
		if (isMoveLeft_ == true)
		{
			if (worldTransform_.rotation_.z > -30.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -1.0f, 1.0f));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z < 0.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -1.0f, 1.0f));
			}
		}
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		if (bulletNum_ == 2)
		{

		}

		dalayTimer_ -= 0.1f;

		//自キャラの座標をコピー
		Vector3 position = GetWorldPosition();

		//弾の速度
		if (isChangeDir_ == false)
		{
			kBulletSpeed_ = 1.0f;
			bulletDir_ = 0;
		}
		else
		{
			kBulletSpeed_ = -1.0f;
			bulletDir_ = 1;
		}
		Vector3 velocity(0, 0, kBulletSpeed_);

		//クールタイムが０になったとき
		if (dalayTimer_ <= 0)
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->PlayerBulletInitialize(position, velocity, bulletDir_);

			//コライダーの追加
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

			//弾種類をセット
			newBullet->SetBulletNum(bulletNum_);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer_ = 0.6f;
		}
	}
}

void Player::Damage()
{
	if (isHit_ == true && isInv_ == false)
	{
		hp_ -= 1;
		isInv_ = true;
		if (hp_ <= 0)
		{
			isDead_ = true;
		}
	}
}

void Player::ChangeBullet()
{

	if (input_->TriggerKey(DIK_B) && !input_->PushKey(DIK_SPACE))
	{
		if (bulletNum_ == 0)
		{
			bulletNum_ = 1;
		}
		else if (bulletNum_ == 1)
		{
			bulletNum_ = 2;
		}
		else if (bulletNum_ == 2)
		{
			bulletNum_ = 0;
		}
	}
}

void Player::BulletUpdate()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

}

void Player::BulletDraw(ViewProjection* viewProjection_)
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}