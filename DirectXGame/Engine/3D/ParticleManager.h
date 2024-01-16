/**
* @file ParticleManager.cpp
* @brief パーティクル管理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Particle.h"
#include "ViewProjection.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <random>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

/// 3Dオブジェクト
class ParticleManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
		Matrix4 matBillboard;	//ビルボード行列
	};
public: // 静的メンバ関数
	/// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// 描画後処理
	static void PostDraw();

	/// 3Dオブジェクト生成
	static ParticleManager* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState_;

private:// 静的メンバ関数

	/// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

public: // メンバ関数
	bool Initialize();
	/// 毎フレーム処理
	void Update();

	/// 描画
	void Draw();

	/// パーティクル発射
	void Fire(Particle* particle, int life, const Vector3& pos, int setNum,bool isStop, const Vector2& setScale);

	//player関係のエフェクト
	void ChasePlayer(Particle* particle, int life,Vector3 pos, const Vector2& setScale);
	//player爆発前のエフェクト
	void PlayerExBefore(Particle* particle, int life,Vector3 pos, int setNum, const Vector2& setScale);
	//player爆発後のエフェクト
	void PlayerExAfter(Particle* particle, int life, Vector3 pos, int setNum, const Vector2& setScale);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	//スケール
	Vector3 scale = { 1,1,1 };

	//ViewProjection
	ViewProjection* viewProjection_;
	//パーティクル
	Particle* particle_;

public://setter
	//パーティクルモデル
	void SetParticleModel(Particle* particleModel) { this->particle_ = particleModel; }
	//カメラ
	void SetViewProjection(ViewProjection* viewProjection) { this->viewProjection_ = viewProjection; }
};