﻿#pragma once
#include <DirectXMath.h>
#include <wrl.h>

class XMViewProjection
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	XMViewProjection();
	~XMViewProjection();
	//更新
	void Update();
	//ビュー行列更新
	void UpdateViewMatrix();
	//プロジェクション行列更新
	void UpdateProjectionMatrix();
	//カメラ移動
	void CameraMoveVector(const XMFLOAT3& move);
	//カメラ視点移動
	void CameraMoveVectorEye(const XMFLOAT3& move);

protected:
	// ビュー行列
	XMMATRIX matView_ = {};
	// 射影行列
	XMMATRIX matProjection_ = {};

	XMMATRIX matViewProjection_ = {};

	// ビルボード行列
	XMMATRIX matBillboard_ = {};
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY_ = {};
	// 視点座標
	XMFLOAT3 eye_ = { 0.0f, 0.0f, -100.0f };
	// 注視点座標
	XMFLOAT3 target_ = { 0.0f,0.0f,0.0f };
	// 上方向ベクトル
	XMFLOAT3 up_ = { 0.0f,1.0f,0.0f };

public://アクセッサ
	//ビュー行列
	const XMMATRIX& GetMatView() { return matView_; }

	//プロジェクション行列
	const XMMATRIX& GetMatProjection() { return matProjection_; }

	const XMMATRIX& GetMatViewProjection() { return matViewProjection_; }

	//ビルボード行列
	const XMMATRIX& GetMatBillboard() { return matBillboard_; }
	const XMMATRIX& GetMatBillboardY() { return matBillboardY_; }

	//視点
	const XMFLOAT3& GetEye() { return eye_; }
	void SetEye(const XMFLOAT3& eye);

	//注視点
	const XMFLOAT3& GetTarget() { return target_; }
	void SetTarget(const XMFLOAT3& target);

	//上方向ベクトル
	const XMFLOAT3& GetUp() { return up_; }
	void SetUp(const XMFLOAT3& up);
};
