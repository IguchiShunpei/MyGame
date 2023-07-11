#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

//デストラクタ
Player::~Player() {
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
	SetPosition(Vector3(0, -2, 0));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	//フラグ
	isBurst = false;
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

void Player::Move()
{
	input = Input::GetInstance();

	if (input->PushKey(DIK_UP))
	{
		SetPosition(GetPosition() + Vector3(0, 0.2f, 0));
	}
	if (input->PushKey(DIK_RIGHT))
	{
		SetPosition(GetPosition() + Vector3(0.2f, 0, 0));
	}
	if (input->PushKey(DIK_LEFT))
	{
		SetPosition(GetPosition() + Vector3(-0.2f, 0, 0));
	}
	if (input->PushKey(DIK_DOWN))
	{
		SetPosition(GetPosition() + Vector3(0, -0.2f, 0));
	}

	//移動限界座標
	const float kMoveLimitX = 6.4f * 1.7f;
	const float kMoveLimitY = 3.25f * 1.7f;

	//範囲を超えない処理
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, +kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitY);
	worldTransform_.position_.y = min(worldTransform_.position_.y, +kMoveLimitY);
}

void Player::Attack()
{
	if (input->PushKey(DIK_SPACE)) {

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

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 1.0f;
		}

	}
}

void Player::ChangeBullet()
{

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
