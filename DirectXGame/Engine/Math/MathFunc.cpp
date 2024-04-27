/**
* @file MathFunc.cpp
* @brief 物理挙動やイージングなどの処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#include "MathFunc.h"

float MathFunc::easeInSine(float a)
{ 
	return 1.0f - cos((a * PI()) / 2.0f);
}

float MathFunc::easeOutSine(float a)
{
	return sin((a * PI()) / 2.0f);
}

float MathFunc::easeInOutSine(float a)
{
	return cos((PI() * a) - 1.0f) / 2.0f;
}

float MathFunc::easeInBack(float a)
{
	const float c1 = 1.70158f;
	const float  c3 = c1 + 1.0f;

	return c3 * a * a * a - c1 * a * a;
}

float MathFunc::easeOutBack(float a)
{
	const float c1 = 1.70158f;
	const float  c3 = c1 + 1.0f;

	return 1 + c3 * (float)pow(a - 1, 3) + c1 * (float)pow(a - 1, 2);
}

float MathFunc::PI()
{
	return 3.141592f;
}

float MathFunc::Degree90()
{
	return 90.0f;
}

float MathFunc::Degree180()
{
	return 180.0f;
}

void MathFunc::HorizontalProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//速度を計算(鉛直投げ上げ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//translationにspeedを加算する
	worldTransform.position_ += speed;

	//アフィン変換用の行列
	AffinMat affinMat;

	//Positionの情報を入れる
	affinMat.translate.m[3][0] = worldTransform.position_.x;
	affinMat.translate.m[3][1] = worldTransform.position_.y;
	affinMat.translate.m[3][2] = worldTransform.position_.z;

	//matWorldに単位行列を入れる
	worldTransform.matWorld_ = Matrix4::identity();

	//行列の計算
	worldTransform.matWorld_ *= affinMat.translate;

	//行列の転送
	worldTransform.UpdateMatrix();

	//フレームを追加
	flame++;
}

void MathFunc::CurveProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//速度を計算(鉛直投げ上げ)
	speed.x = startSpeed.x + G * (static_cast<float>(flame) / 60.0f);

	//translationにspeedを加算する
	worldTransform.position_ += speed;

	//アフィン変換用の行列
	AffinMat affinMat;

	//Positionの情報を入れる
	affinMat.translate.m[3][0] = worldTransform.position_.x;
	affinMat.translate.m[3][1] = worldTransform.position_.y;
	affinMat.translate.m[3][2] = worldTransform.position_.z;

	//matWorldに単位行列を入れる
	worldTransform.matWorld_ = Matrix4::identity();

	//行列の計算
	worldTransform.matWorld_ *= affinMat.translate;

	//行列の転送
	worldTransform.UpdateMatrix();

	//フレームを追加
	flame++;
}
