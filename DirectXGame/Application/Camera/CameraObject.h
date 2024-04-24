/**
* @file  CameraObject.h
* @brief 始点と終点のオブジェクト
* @author イグチ_シュンペイ
*/

#include "Model.h"
#include "Object3d.h"

#pragma once
class CameraObject : public Object3d
{
public:
	//初期化
	void CObjectInitialize();

	//削除フラグ
	bool GetIsDelete() const { return isDelete_; }
	void SetIsDelete(bool isDelete) { isDelete_ = isDelete; }
	//選択フラグ
	bool GetIsSelect() const { return isSelect_; }
	void SetIsSelect(bool isSelect) { isSelect_ = isSelect; }
	//alpha
	float GetAlpha() const { return alpha_; }
	void SetAlpha(float alpha) { alpha_ = alpha; }
	//色
	Vector3 GetColor() const { return color_; }
	void SetColor(Vector3 color) { color_ = color; }

private:
	// モデル
	std::unique_ptr <Model> cameraObjectModel_;

	//削除フラグ
	bool isDelete_;

	//選択されているかフラグ
	bool isSelect_;

	//alpha値
	float alpha_;
	
	//色
	Vector3 color_;

};
