/**
* @file Particle.cpp
* @brief パーティクルに関するクラス
* @author イグチ_シュンペイ
*/

#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <forward_list>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"


class Particle
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public://サブクラス

	struct VertexPosScale
	{
		Vector3 pos; // xyz座標
		float scale;  //スケール
	};

	//パーティクル一粒
	struct OneParticle
	{
	
		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;

		//スケール
		float scale = 1.0f;
		//初期値
		float s_Scale = 1.0f;
		//最終値
		float e_Scale = 0.0f;
	};

private://定数
	static const int vertexCount = 1024;		// 頂点数

public:
	static Particle* GetInstance();

public://メンバ関数

	//OBJファイルからパーティクルモデルを読み込む
	static Particle* LoadParticleTexture(const std::string& fileName);
	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();
	// テクスチャ読み込み
	void LoadTexture(const std::string& fileName);

	//各種バッファ生成
	void CreateBuffers();
	//更新
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//パーティクルの追加
	void Add(int life, Vector3 position, Vector3 velocity,
		float start_scale, float end_scale);

private://メンバ変数
	// デバイス
	static ID3D12Device* device_;
	//デフォルトテクスチャ格納ディレクトリ
	static std::string defaultTextureDirectoryPath_;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize_;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	//頂点データ配列
	VertexPosScale vertices_[vertexCount];

	std::forward_list<OneParticle> particles_;

public://setter
	//デバイス
	static void SetDevice(ID3D12Device* device) { Particle::device_ = device; }

};