#include "Vector4.h"

Vector4::Vector4()
	:x(0), y(0), z(0), w(0)
{
}

Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

float Vector4::Length() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector4& Vector4::Normalize()
{
	float len = Length();

	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

float Vector4::Dot(const Vector4& v)const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

Vector4 Vector4::Cross(const Vector4& v)const
{
	Vector4 temp;

	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	temp.w = v.w;

	return temp;
}

Vector4 Vector4::operator+()const
{
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-()const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

Vector4& Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}

Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;

	return *this;
}

const Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);

	return temp += v2;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);

	return temp -= v2;
}

const Vector4 operator*(const Vector4& v, float s)
{
	Vector4 temp(v);

	return temp *= s;
}

const Vector4 operator*(float s, const Vector4& v)
{
	return v * s;
}

const Vector4 operator/(const Vector4& v, float s)
{
	Vector4 temp(v);

	return temp /= s;
}