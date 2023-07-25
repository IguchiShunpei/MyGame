#include "MathFunc.h"

float MathFunc::easeInSine(float a)
{ 
	const float PI = 3.141592f;
	return 1.0f - cos((a * PI) / 2);
}

float MathFunc::easeOutSine(float a)
{
	const float PI = 3.141592f;
	return sin((a * PI) / 2.0f);
}

float MathFunc::easeInOutSine(float a)
{
	const float PI = 3.141592f;
	return cos((PI * a) - 1.0f) / 2.0f;
}

void MathFunc::HorizontalProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//���x���v�Z(���������グ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//translation��speed�����Z����
	worldTransform.position_ += speed;

	//�A�t�B���ϊ��p�̍s��
	AffinMat affinMat;

	//Position�̏�������
	affinMat.translate.m[3][0] = worldTransform.position_.x;
	affinMat.translate.m[3][1] = worldTransform.position_.y;
	affinMat.translate.m[3][2] = worldTransform.position_.z;

	//matWorld�ɒP�ʍs�������
	worldTransform.matWorld_ = Matrix4::identity();

	//�s��̌v�Z
	worldTransform.matWorld_ *= affinMat.translate;

	//�s��̓]��
	worldTransform.UpdateMatrix();

	//�t���[����ǉ�
	flame++;
}

void MathFunc::CurveProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//���x���v�Z(���������グ)
	speed.x = startSpeed.x + G * (static_cast<float>(flame) / 60.0f);

	//translation��speed�����Z����
	worldTransform.position_ += speed;

	//�A�t�B���ϊ��p�̍s��
	AffinMat affinMat;

	//Position�̏�������
	affinMat.translate.m[3][0] = worldTransform.position_.x;
	affinMat.translate.m[3][1] = worldTransform.position_.y;
	affinMat.translate.m[3][2] = worldTransform.position_.z;

	//matWorld�ɒP�ʍs�������
	worldTransform.matWorld_ = Matrix4::identity();

	//�s��̌v�Z
	worldTransform.matWorld_ *= affinMat.translate;

	//�s��̓]��
	worldTransform.UpdateMatrix();

	//�t���[����ǉ�
	flame++;
}
