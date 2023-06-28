#pragma once

#include <cmath>

class Vector4
{
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分
	float w; // w成分

public: // コンストラクタ
	Vector4();
	Vector4(float x, float y, float z, float w);

public: // メンバ関数
	float Length() const;                 // ノルム(長さ)を求める
	Vector4& Normalize();                 // 正規化する
	float Dot(const Vector4& v)const;     // 内積を求める
	Vector4 Cross(const Vector4& v)const; // 外積を求める

	// 単項演算子のオーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	// 代入演算子のオーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};
// 2項演算子オーバーロード
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);