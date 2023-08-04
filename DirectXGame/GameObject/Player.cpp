#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

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

	bulletNum_ = 0;
	initMotionTime_ = 0.0f;

	//フラグ
	isBurst_ = false;
	isInit_ = false;
	isHit_ = false;
	isDownRotation_ = false;
	isUpRotation_ = false;
	isRightRotation_ = false;
	isLeftRotation_ = false;
}

void Player::Update()
{
	input_ = Input::GetInstance();

	Move();

	Rotate();

	ChangeBullet();

	Attack();

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

void Player::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class EnemyBullet";
	//相手がEnemyBullet
	if (strcmp(toCollisionName, str1) == 0) 
	{
		isHit_ = true;
	}
}

void Player::IntitMotion()
{
	if (isInit_ == false)
	{
		//前進
		worldTransform_.position_.z += 0.5f;
		//イージングを使った回転
		worldTransform_.rotation_.z = 360 * -MathFunc::easeOutSine(initMotionTime_ / 40.0f);
		initMotionTime_++;
		// ワールドトランスフォームの行列更新と転送
		worldTransform_.UpdateMatrix();
		if (worldTransform_.position_.z >= 0)
		{
			SetPosition(Vector3(0, -2, 0));
			SetRotation(Vector3(0, 0, 0));
			isInit_ = true;
		}
	}
}

void Player::Move()
{
	input_ = Input::GetInstance();

	//上キーを押したとき
	if (input_->PushKey(DIK_UP))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0.0f, 0.2f, 0.0f));
		//傾きフラグ
		isUpRotation_ = true;
	}
	else
	{
		isUpRotation_ = false;
	}
	//下キーを押したとき
	if (input_->PushKey(DIK_DOWN))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0, -0.2f, 0));
		//傾きフラグ
		isDownRotation_ = true;
	}
	else
	{
		isDownRotation_ = false;
	}
	//右キーを押したとき
	if (input_->PushKey(DIK_RIGHT))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0.2f, 0.0f, 0.0f));
		//傾きフラグ
		isRightRotation_ = true;
	}
	else
	{
		isRightRotation_ = false;
	}
	//左キーを押したとき
	if (input_->PushKey(DIK_LEFT))
	{
		//移動
		SetPosition(GetPosition() + Vector3(-0.2f, 0, 0));
		//傾きフラグ
		isLeftRotation_ = true;
	}
	else
	{
		isLeftRotation_ = false;
	}

	//移動限界座標
	const float kMoveLimitX = 6.0f * 1.7f;
	const float kMoveLimitY = 4.0f * 1.7f;

	//範囲を超えない処理
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, +kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitY);
	worldTransform_.position_.y = min(worldTransform_.position_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	//上キーを押したときの傾き処理
	if (isUpRotation_ == true)
	{
		if (worldTransform_.rotation_.x > -45.0f)
		{
			SetRotation(GetRotation() - Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x < 0.0f)
		{
			SetRotation(GetRotation() + Vector3(2.0f, 0.0f, 0.0f));
		}
	}
	//下キーを押したときの傾き処理
	if (isDownRotation_ == true)
	{
		if (worldTransform_.rotation_.x < 45.0f)
		{
			SetRotation(GetRotation() + Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x > 0.0f)
		{
			SetRotation(GetRotation() - Vector3(2.0f, 0.0f, 0.0f));
		}
	}
	//右キーを押したとき
	if (isRightRotation_ == true)
	{
		if (worldTransform_.rotation_.z > -45.0f)
		{
			SetRotation(GetRotation() - Vector3(0.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.z < 0.0f)
		{
			SetRotation(GetRotation() + Vector3(0.0f, 0.0f, 2.0f));
		}
	}
	//左キーを押したとき
	if (isLeftRotation_ == true)
	{
		if (worldTransform_.rotation_.z < 45.0f)
		{
			SetRotation(GetRotation() + Vector3(0.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.z > 0.0f)
		{
			SetRotation(GetRotation() - Vector3(0.0f, 0.0f, 2.0f));
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
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//クールタイムが０になったとき
		if (dalayTimer_ <= 0)
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->PlayerBulletInitialize(position, velocity);

			//コライダーの追加
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

			//弾種類をセット
			newBullet->SetBulletNum(bulletNum_);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer_ = 1.2f;
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