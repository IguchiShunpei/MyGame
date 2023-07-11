#include "Player.h"
#include "SphereCollider.h"
#include "string.h"
#include "GameScene.h"

//�f�X�g���N�^
Player::~Player() {
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
	SetPosition(Vector3(0, -2, 0));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	//�t���O
	isBurst = false;
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

	//�ړ����E���W
	const float kMoveLimitX = 6.4f * 1.7f;
	const float kMoveLimitY = 3.25f * 1.7f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.position_.x = max(worldTransform_.position_.x, -kMoveLimitX);
	worldTransform_.position_.x = min(worldTransform_.position_.x, +kMoveLimitX);
	worldTransform_.position_.y = max(worldTransform_.position_.y, -kMoveLimitY);
	worldTransform_.position_.y = min(worldTransform_.position_.y, +kMoveLimitY);
}

void Player::Attack()
{
	if (input->PushKey(DIK_SPACE)) {

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

			//���̓o�^
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
