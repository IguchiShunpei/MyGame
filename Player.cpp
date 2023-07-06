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
bool Player::PlayerInitialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	Initialize();

	// OBJ���烂�f���f�[�^��ǂݍ���
	playerModel = Model::LoadFromOBJ("player");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(playerModel);
	SetPosition(Vector3(0, -2, 0));

	//�t���O
	isAttack = false;

	return true;
}

void Player::Update()
{
	input = Input::GetInstance();

	Move();

	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
		bullet->ColliderUpdate();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
	{
		return bullet->GetIsDelete();
	});

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
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
	const float kMoveLimitX = 6.4f;
	const float kMoveLimitY = 3.25f;

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
			newBullet->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.0f));

			//���̓o�^
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 1.0f;
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
