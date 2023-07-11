#pragma once
#include "WorldTransform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <math.h>

namespace MathFunc
{
	struct AffinMat 
	{
		Matrix4 scale     = Matrix4::identity();
		Matrix4 rotateX   = Matrix4::identity();
		Matrix4 rotateY   = Matrix4::identity();
		Matrix4 rotateZ   = Matrix4::identity();
		Matrix4 translate = Matrix4::identity();
	};

	//イージング
	static float easeInSine(float a);

	static float easeOutSine(float a);

	static float easeInOutSine(float a);

	//水平投射
	void HorizontalProjection(WorldTransform& worldTransform, Vector3& startSpeed, float G, float& flame);
};

