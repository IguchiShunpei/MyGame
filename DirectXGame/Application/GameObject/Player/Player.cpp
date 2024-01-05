/**
* @file Player.cpp
* @brief 自機の処理クラス
* @author イグチ_シュンペイ
*/

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
	input_ = Input::GetInstance();
	
	dxCommon_ = DirectXCommon::GetInstance();
	
	Initialize();

	// OBJからモデルデータを読み込む
	playerModel_ = Model::LoadFromOBJ("fighter");
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(playerModel_);

    //レティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice());

	distancePlayerToReticle_ = 15.0f;
	reticleAlpha_ = 0.0f;
	reticleAlphaNum_ = 0.2f;
	reticleAlphaNumMax_ = 1.0f;
	reticleAlphaNumMin_ = 0.0f;
	isInvicibleReticle_ = false;

	//レティクル
	reticle_.Initialize(dxCommon_->GetDevice(), 0, Vector2(50.0f,50.0f), false, false);
	reticle_.SetScale({ 100,100 });
	reticle_.SetPosition({ 0,0,0 });
	reticle_.SetAlpha(reticle_, reticleAlpha_);
	reticle_.SpriteTransferVertexBuffer(reticle_, 0);
	reticle_.Update(reticle_, spriteCommon_);
	reticle_.LoadTexture(spriteCommon_, 0, L"Resources/2d/reticle.png", dxCommon_->GetDevice());


	//コライダー関係の変数
	colliderPos_ = {0.0f,0.0f,0.0f};
	playerColliderRadius_ = 1.0f;
	bulletColliderRadius_ = 0.5f;

	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(colliderPos_, playerColliderRadius_));
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
	moveRota_ = 0.5f;
	moveRotaMax_ = 30.0f;
	bulletLevel_ = 1;
	initMotionTime_ = 0.0f;
	dalayTimer_ = 0.0f;
	hp_ = 7;
	initSpeedZ_ = 0.5f;
	initRotaZ_ = 400.0f;
	initMotionTimeMax_ = 40.0f;

	bulletPower_ = 1;
	kBulletSpeed_ = 3.0f;

	initPos_ = { 0.0f,-2.0f,0.0f };
	initRota_ = { 0.0f,0.0f,0.0f };
	initRotaMax_ = 360.0f;

	backRota_ = 4.0f;

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

void Player::Update(ViewProjection* viewProjection_)
{

	if (isDead_ == false)
	{
		ReticleUpdate(viewProjection_);

		Move();

		Rotate();

		BulletPowerUp();

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
	const char* str3 = "class BossEnemy";

	//相手がEnemy
	if (isInv_ == false)
	{
		if (strcmp(toCollisionName, str1) == 0 || strcmp(toCollisionName, str2) == 0 || strcmp(toCollisionName, str3) == 0)
		{
			isHit_ = true;
		}
	}
	//ダメージ処理
	Damage();
}

void Player::InitMotion()
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
			SetPosition(initPos_);
			isInitAfter_ = true;
		}
	}
	//余分に回転した分を戻す処理
	if (isInitAfter_ == true)
	{
		worldTransform_.rotation_.z += backRota_;
		//戻り切ったらフラグを切り替え
		if (worldTransform_.rotation_.z >= -initRotaMax_)
		{
			SetRotation(initRota_);
			isInitAfter_ = false;
			isInit_ = true;
		}
	}
	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void Player::Shake(float x,float y)
{
	//乱数生成装置
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float>dist(-x, x);
	std::uniform_real_distribution<float>dist2(-y, y);

	worldTransform_.position_ = worldTransform_.position_ + Vector3(dist(engine), dist2(engine), dist2(engine));
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

	//速度変化
	SpeedChange();

	//移動限界座標
	const float kMoveLimitX = 9.0f * 1.7f;
	const float kMoveLimitYUp = 6.0f * 1.7f;
	const float kMoveLimitYDown = 3.0f * 1.7f;

	//範囲を超えない処理
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitYDown);
	worldTransform_.position_.y = min(worldTransform_.position_.y, kMoveLimitYUp);
}

void Player::SpeedChange()
{
	//キーを押しているときに加速
	//キーを離したら減速
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
		if (worldTransform_.rotation_.x > -moveRotaMax_)
		{
			SetRotation(GetRotation() - Vector3(moveRota_, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x < 0.0f)
		{
			SetRotation(GetRotation() + Vector3(moveRota_, 0.0f, 0.0f));
		}
	}
	//下キーを押したときの傾き処理
	if (isMoveDown_ == true)
	{
		if (worldTransform_.rotation_.x < moveRotaMax_)
		{
			SetRotation(GetRotation() + Vector3(moveRota_, 0.0f, 0.0f));
		}
	}
	else
	{
		if (worldTransform_.rotation_.x > 0.0f)
		{
			SetRotation(GetRotation() - Vector3(moveRota_, 0.0f, 0.0f));
		}
	}
	if (isChangeDir_ == false)
	{
		//右キーを押したとき
		if (isMoveRight_ == true)
		{
			if (worldTransform_.rotation_.z > -moveRotaMax_)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z < 0.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		//左キーを押したとき
		if (isMoveLeft_ == true)
		{
			if (worldTransform_.rotation_.z < moveRotaMax_)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z > 0.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
	}
	else
	{
		//右キーを押したとき
		if (isMoveRight_ == true)
		{
			if (worldTransform_.rotation_.z < moveRotaMax_)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z > 0.0f)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		//左キーを押したとき
		if (isMoveLeft_ == true)
		{
			if (worldTransform_.rotation_.z > -moveRotaMax_)
			{
				SetRotation(GetRotation() - Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
		else
		{
			if (worldTransform_.rotation_.z < 0.0f)
			{
				SetRotation(GetRotation() + Vector3(0.0f, -moveRota_, moveRota_));
			}
		}
	}
}

void Player::Attack()
{
	//Spaceキーを押したとき
	if (input_->TriggerKey(DIK_SPACE))
	{
		//自キャラの座標をコピー
		Vector3 position = GetWorldPosition();

		//弾の速度
		Vector3 velocity(0, 0, kBulletSpeed_);

		//自機の向いてる方向に弾を撃つ
		velocity = bVelocity(velocity, worldTransform_);
	
		//レティクルのワールド座標を取得
		Vector3 reticleWorldPos;

		reticleWorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
		reticleWorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
		reticleWorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

		//自機から標準オブジェクトへのベクトル
		velocity = reticleWorldPos - GetWorldPosition();

		//速度と向きを合成
		velocity = velocity.normalize() * kBulletSpeed_;

		//球の生成
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		//球の初期化
		newBullet->PlayerBulletInitialize(position, velocity, bulletDir_,bulletLevel_);

		//コライダーの追加
		newBullet->SetCollider(new SphereCollider(colliderPos_, bulletColliderRadius_));

		//球の登録
		bullets_.push_back(std::move(newBullet));

	}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

Vector3 Player::MatVector(Vector3 v, Matrix4 mat)
{
	Vector3 pos;
	pos.x = mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * 1;
	pos.y = mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * 1;
	pos.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * 1;

	return pos;
}

Matrix4 Player::SetViewport(const Vector3& v)
{
	//単位行列の設定
	Matrix4 matViewport = Matrix4::identity();
	matViewport.m[0][0] = 1280.0f / 2.0f;
	matViewport.m[1][1] = -720.0f / 2.0f;
	matViewport.m[3][0] = (1280.0f / 2.0f) + v.x;
	matViewport.m[3][1] = (720.0f / 2.0f) + v.y;
	return matViewport;
}

Vector3 Player::Division(const Vector3& v, Matrix4 mat)
{
	Vector4 devision;
	devision.x = mat.m[0][0] * v.x + mat.m[1][0] * v.y + mat.m[2][0] * v.z + mat.m[3][0] * 1;
	devision.y = mat.m[0][1] * v.x + mat.m[1][1] * v.y + mat.m[2][1] * v.z + mat.m[3][1] * 1;
	devision.z = mat.m[0][2] * v.x + mat.m[1][2] * v.y + mat.m[2][2] * v.z + mat.m[3][2] * 1;
	devision.w = mat.m[0][3] * v.x + mat.m[1][3] * v.y + mat.m[2][3] * v.z + mat.m[3][3] * 1;

	devision.x = devision.x / devision.w;
	devision.y = devision.y / devision.w;
	devision.z = devision.z / devision.w;

	return { devision.x, devision.y, devision.z };
}

void Player::Damage()
{
	if (isHit_ == true && isInv_ == false)
	{
		hp_--;
		isInv_ = true;
		if (hp_ <= 0)
		{
			isDead_ = true;
		}
	}
}

void Player::BulletPowerUp()
{
    //ダメージ量を変更
	switch (bulletLevel_)
	{
	case 1:
		bulletPower_ = 1;
		break;
	case 2:
		bulletPower_ = 3;
		break;
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

void Player::ReticleUpdate(ViewProjection* viewProjection_)
{
	if (isInvicibleReticle_ == false)
	{
		if (reticleAlpha_ < reticleAlphaNumMax_)
		{
			reticleAlpha_ += reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	else
	{
		if (reticleAlpha_ > reticleAlphaNumMin_)
		{
			reticleAlpha_ -= reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offSet = { 0,0,1.0f };
		//自機のワールド行列の回転を反映
		offSet = MatVector(offSet, worldTransform_.matWorld_);
		//長さを整える
		offSet.normalize() * distancePlayerToReticle_;
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.position_ = Vector3::AddVector3(GetWorldPosition(), offSet);
		worldTransform3DReticle_.UpdateMatrix();
	}
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// 3Dレティクルのワールド行列から,ワールド座標を取得
		Vector3 reticleWorldPos;

		reticleWorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
		reticleWorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
		reticleWorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
		//ビューポート行列
		Matrix4 matViewport = SetViewport(Vector3(0, 0, 0));

		//ビュー行列とプロジェクション行列,ビューポート行列を合成する
		Matrix4 matViewprojectionViewport =
			viewProjection_->matView * viewProjection_->matProjection * matViewport;
		//ワールド→スクリーン座標変換
		reticleWorldPos = Division(reticleWorldPos, matViewprojectionViewport);
		//座標設定
		reticle_.SetPosition(Vector3(reticleWorldPos.x, reticleWorldPos.y,0.0f));
	}
	reticle_.Update(reticle_, spriteCommon_);
}

void Player::ReticleDraw()
{
	reticle_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
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