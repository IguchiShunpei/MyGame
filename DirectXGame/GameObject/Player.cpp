#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

//デストラクタ
Player::~Player() 
{
	delete playerModel;
	delete playerBullet;
}

//初期化
void Player::PlayerInitialize()
{
	Initialize();

	// OBJからモデルデータを読み込む
	playerModel = Model::LoadFromOBJ("fighter");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerModel);
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
	SetPosition(Vector3(0, -2, -20));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	bulletNum = 0;
	initMotionTime = 0.0f;

	//フラグ
	isBurst = false;
	isInit = false;
	isDownRotation = false;
	isUpRotation = false;
	isRightRotation = false;
	isLeftRotation = false;
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	input = Input::GetInstance();

	Move();

	Rotate();

	ChangeBullet();

	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

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

void Player::IntitMotion()
{
	if (isInit == false)
	{
		//前進
		worldTransform_.position_.z += 0.5f;
		//イージングを使った回転
		worldTransform_.rotation_.z = 360 * -MathFunc::easeOutSine(initMotionTime / 40.0f);
		initMotionTime++;
		// ワールドトランスフォームの行列更新と転送
		worldTransform_.UpdateMatrix();
		if (worldTransform_.position_.z >= 0)
		{
			SetPosition(Vector3(0, -2, 0));
			SetRotation(Vector3(0, 0, 0));
			isInit = true;
		}
	}
}

void Player::Move()
{
	input = Input::GetInstance();

	//上キーを押したとき
	if (input->PushKey(DIK_UP))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0.0f, 0.2f, 0.0f));
		//傾きフラグ
		isUpRotation = true;
	}
	else
	{
		isUpRotation = false;
	}
	//下キーを押したとき
	if (input->PushKey(DIK_DOWN))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0, -0.2f, 0));
		//傾きフラグ
		isDownRotation = true;
	}
	else
	{
		isDownRotation = false;
	}
	//右キーを押したとき
	if (input->PushKey(DIK_RIGHT))
	{
		//移動
		SetPosition(GetPosition() + Vector3(0.2f, 0.0f, 0.0f));
		//傾きフラグ
		isRightRotation = true;
	}
	else
	{
		isRightRotation = false;
	}
	//左キーを押したとき
	if (input->PushKey(DIK_LEFT))
	{
		//移動
		SetPosition(GetPosition() + Vector3(-0.2f, 0, 0));
		//傾きフラグ
		isLeftRotation = true;
	}
	else
	{
		isLeftRotation = false;
	}

	//移動限界座標
	const float kMoveLimitX = 6.8f * 1.7f;
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
	if (isUpRotation == true)
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
	if (isDownRotation == true)
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
	if (isRightRotation == true)
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
	if (isLeftRotation == true)
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
	if (input->PushKey(DIK_SPACE))
	{
		if (bulletNum == 2)
		{

		}

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
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->PlayerBulletInitialize(position, velocity);

			//コライダーの追加
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

			//弾種類をセット
			newBullet->SetBulletNum(bulletNum);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 1.0f;
		}
	}
}

void Player::ChangeBullet()
{

	if (input->TriggerKey(DIK_B) && !input->PushKey(DIK_SPACE))
	{
		if (bulletNum == 0)
		{
			bulletNum = 1;
		}
		else if (bulletNum == 1)
		{
			bulletNum = 2;
		}
		else if (bulletNum == 2)
		{
			bulletNum = 0;
		}
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