#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

//�f�X�g���N�^
Player::~Player() 
{
	delete playerModel_;
	delete playerBullet_;
}

//������
void Player::PlayerInitialize()
{
	Initialize();

	// OBJ���烂�f���f�[�^��ǂݍ���
	playerModel_ = Model::LoadFromOBJ("fighter");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(playerModel_);
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
	SetPosition(Vector3(0, -2, -20));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	bulletNum_ = 0;
	initMotionTime_ = 0.0f;

	//�t���O
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

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
}

void Player::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	const char* str1 = "class EnemyBullet";
	//���肪EnemyBullet
	if (strcmp(toCollisionName, str1) == 0) 
	{
		isHit_ = true;
	}
}

void Player::IntitMotion()
{
	if (isInit_ == false)
	{
		//�O�i
		worldTransform_.position_.z += 0.5f;
		//�C�[�W���O���g������]
		worldTransform_.rotation_.z = 360 * -MathFunc::easeOutSine(initMotionTime_ / 40.0f);
		initMotionTime_++;
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
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

	//��L�[���������Ƃ�
	if (input_->PushKey(DIK_UP))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0.0f, 0.2f, 0.0f));
		//�X���t���O
		isUpRotation_ = true;
	}
	else
	{
		isUpRotation_ = false;
	}
	//���L�[���������Ƃ�
	if (input_->PushKey(DIK_DOWN))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0, -0.2f, 0));
		//�X���t���O
		isDownRotation_ = true;
	}
	else
	{
		isDownRotation_ = false;
	}
	//�E�L�[���������Ƃ�
	if (input_->PushKey(DIK_RIGHT))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0.2f, 0.0f, 0.0f));
		//�X���t���O
		isRightRotation_ = true;
	}
	else
	{
		isRightRotation_ = false;
	}
	//���L�[���������Ƃ�
	if (input_->PushKey(DIK_LEFT))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(-0.2f, 0, 0));
		//�X���t���O
		isLeftRotation_ = true;
	}
	else
	{
		isLeftRotation_ = false;
	}

	//�ړ����E���W
	const float kMoveLimitX = 6.0f * 1.7f;
	const float kMoveLimitY = 4.0f * 1.7f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, +kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitY);
	worldTransform_.position_.y = min(worldTransform_.position_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	//��L�[���������Ƃ��̌X������
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
	//���L�[���������Ƃ��̌X������
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
	//�E�L�[���������Ƃ�
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
	//���L�[���������Ƃ�
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

		//���L�����̍��W���R�s�[
		Vector3 position = GetWorldPosition();

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//�N�[���^�C�����O�ɂȂ����Ƃ�
		if (dalayTimer_ <= 0)
		{
			//���̐���
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//���̏�����
			newBullet->PlayerBulletInitialize(position, velocity);

			//�R���C�_�[�̒ǉ�
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

			//�e��ނ��Z�b�g
			newBullet->SetBulletNum(bulletNum_);

			//���̓o�^
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
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	//�e�X�V
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