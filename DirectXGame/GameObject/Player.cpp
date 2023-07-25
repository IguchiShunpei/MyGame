#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

//�f�X�g���N�^
Player::~Player() 
{
	delete playerModel;
	delete playerBullet;
}

//������
void Player::PlayerInitialize()
{
	Initialize();

	// OBJ���烂�f���f�[�^��ǂݍ���
	playerModel = Model::LoadFromOBJ("fighter");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(playerModel);
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));
	SetPosition(Vector3(0, -2, -20));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	bulletNum = 0;
	initMotionTime = 0.0f;

	//�t���O
	isBurst = false;
	isInit = false;
	isDownRotation = false;
	isUpRotation = false;
	isRightRotation = false;
	isLeftRotation = false;
}

void Player::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->GetIsDelete();
		});

	input = Input::GetInstance();

	Move();

	Rotate();

	ChangeBullet();

	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

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

void Player::IntitMotion()
{
	if (isInit == false)
	{
		//�O�i
		worldTransform_.position_.z += 0.5f;
		//�C�[�W���O���g������]
		worldTransform_.rotation_.z = 360 * -MathFunc::easeOutSine(initMotionTime / 40.0f);
		initMotionTime++;
		// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
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

	//��L�[���������Ƃ�
	if (input->PushKey(DIK_UP))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0.0f, 0.2f, 0.0f));
		//�X���t���O
		isUpRotation = true;
	}
	else
	{
		isUpRotation = false;
	}
	//���L�[���������Ƃ�
	if (input->PushKey(DIK_DOWN))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0, -0.2f, 0));
		//�X���t���O
		isDownRotation = true;
	}
	else
	{
		isDownRotation = false;
	}
	//�E�L�[���������Ƃ�
	if (input->PushKey(DIK_RIGHT))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(0.2f, 0.0f, 0.0f));
		//�X���t���O
		isRightRotation = true;
	}
	else
	{
		isRightRotation = false;
	}
	//���L�[���������Ƃ�
	if (input->PushKey(DIK_LEFT))
	{
		//�ړ�
		SetPosition(GetPosition() + Vector3(-0.2f, 0, 0));
		//�X���t���O
		isLeftRotation = true;
	}
	else
	{
		isLeftRotation = false;
	}

	//�ړ����E���W
	const float kMoveLimitX = 6.8f * 1.7f;
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
	//���L�[���������Ƃ��̌X������
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
	//�E�L�[���������Ƃ�
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
	//���L�[���������Ƃ�
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

		//���L�����̍��W���R�s�[
		Vector3 position = GetWorldPosition();

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//�N�[���^�C�����O�ɂȂ����Ƃ�
		if (dalayTimer <= 0)
		{
			//���̐���
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//���̏�����
			newBullet->PlayerBulletInitialize(position, velocity);

			//�R���C�_�[�̒ǉ�
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 0.5f));

			//�e��ނ��Z�b�g
			newBullet->SetBulletNum(bulletNum);

			//���̓o�^
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