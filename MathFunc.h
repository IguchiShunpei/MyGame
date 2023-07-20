#pragma once
#include "WorldTransform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <math.h>

class MathFunc
{
public:
	struct AffinMat 
	{
		Matrix4 scale     = Matrix4::identity();
		Matrix4 rotateX   = Matrix4::identity();
		Matrix4 rotateY   = Matrix4::identity();
		Matrix4 rotateZ   = Matrix4::identity();
		Matrix4 translate = Matrix4::identity();
	};

	//�C�[�W���O
	static float easeInSine(float a);

	static float easeOutSine(float a);

	static float easeInOutSine(float a);

	//��������
	static void HorizontalProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame);

	//�J�[�u
	static void CurveProjection(WorldTransform& worldTransform, const Vector3& startSpeed, float G, float& flame);
};

