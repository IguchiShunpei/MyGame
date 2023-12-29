/**
* @file Spline.cpp
* @brief Spline曲線の計算処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Vector3.h"
#include <Vector>

class Spline
{
public:
	//初期化
	void Initialize();

	//更新
	Vector3 Update(std::vector<Vector3>& points, float val);

	//制御点の集合(vectorコンテナ)、補間する区間の添字、時間経過率
	Vector3 SplinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

private:
	//時間計算に必要なデータ
	long long startCount;
	long long nowCount;
	double elapsedCount = 0;

	//全体時間[s]
	float maxTime = 5.0f;				

	//点の位置
	Vector3 position_;

	//P1からスタートする
	size_t startIndex = 1;
	//終了フラグ
	bool isEnd;
	float t;
};