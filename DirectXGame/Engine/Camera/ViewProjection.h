/**
* @file ViewProjection.h
* @brief 自作の数学クラスを用いたカメラクラス
* @author イグチ_シュンペイ
*/

#pragma once
#include"Vector3.h"
#include "Vector4.h"
#include"Matrix4.h"
#include "WinApp.h"
#include<d3d12.h>
#include<wrl.h>

class ViewProjection
{
public:// サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataViewProjection {
		Matrix4 view;       // ワールド → ビュー変換行列
		Matrix4 projection; // ビュー → プロジェクション変換行列
		Vector3 cameraPos;  // カメラ座標（ワールド座標）
	};
public:// メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// 初期化
	void Initialize();

	/// 行列を更新する
	void UpdateMatrix();

	// バッファのゲッター
	ID3D12Resource* GetBuff() { return constBuff.Get(); }

	const Vector3& GetEye() { return eye_; }

	const Matrix4& GetMatViewProjection() { return matViewProjection_; }

	void SetEye(const Vector3& eye) { this->eye_ = eye; }

	void SetTarget(const Vector3& target) { this->target_ = target; }

	void SetUp(const Vector3& up) { this->up_ = up; }

private:// プライベート関数
	// 円周率
	const float PI = 3.141592f;

	/// 定数バッファ生成
	void CreateConstBuffer();

	/// マッピングする
	void Map();

	// 度数からラジアンに変換
	float ToRadian(float angle) { return angle * (PI / 180); }

public:// パブリック変数
#pragma region ビュー行列の設定
	// 視点座標
	Vector3 eye_ = { 0.0f, 0.0f, -20.0f };
	// 注視点座標
	Vector3 target_ = { 0.0f, 0.0f, 100.0f };
	// 上方向ベクトル
	Vector3 up_ = { 0.0f, 1.0f, 0.0f };
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = ToRadian(45.0f);
	// ビューポートのアスペクト比
	float aspectRatio = (float)SIEngine::WinApp::window_width / SIEngine::WinApp::window_height;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 2000.0f;
#pragma endregion

	// ビュー行列
	Matrix4 matView_;
	// 射影行列
	Matrix4 matProjection_;
	// 合成
	Matrix4 matViewProjection_;

private:// メンバ変数

	// デバイス（借りてくる）
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

};

