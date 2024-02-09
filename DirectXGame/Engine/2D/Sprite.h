/**
* @file Sprite.h
* @brief  スプライトに関するクラス
* @author イグチ_シュンペイ
*/

#pragma once
#include <Windows.h>
#include <D3dx12.h>
#include <d3dcompiler.h>
#include "DirectXCommon.h"
#include <cassert>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

#include<DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
#include<array>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

namespace SIEngine
{

	// パイプラインセット
	struct PipelineSet {
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
	};

	// スプライトの共通データ
	struct SpriteCommon {
		// パイプラインセット
		PipelineSet pipelineSet;
		// 射影行列
		Matrix4 matProjection{};
		// テクスチャ用デスクリプタヒープの生成
		ComPtr<ID3D12DescriptorHeap> descHeap;
		// SRVの最大枚数
		static const size_t kMaxSRVCount = 512;
		// テクスチャソース(テクスチャバッファ)の配列
		ComPtr<ID3D12Resource> texBuff[kMaxSRVCount];
		// テクスチャバッファ取得
		ID3D12Resource* GetTexBuffer(uint32_t index) const { return texBuff[index].Get(); }
	};

	class Sprite
	{
	public: // サブクラス

		// 定数バッファ用データ構造体
		struct ConstBufferDataB0
		{
			Vector4 color_;	// 色 (RGBA)
			Matrix4 mat_;	// ３Ｄ変換行列
		};

		// 頂点データ
		struct VertexPosUv {
			Vector3 pos_;
			Vector2 uv_;
		};

	public:
		//インストラクタ
		Sprite();

		// スプライト共通データ生成
		SpriteCommon SpriteCommonCreate(ID3D12Device* device);

		// 3Dオブジェクト用パイプライン生成
		PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* device);

		//生成初期化
		void Initialize(ID3D12Device* device, UINT texNumber, Vector2 anchorpPoint, bool isFlipX, bool isFlipY);

		//更新
		void Update(Sprite& sprite, const SpriteCommon& spriteCommon_);

		//描画前処理
		static void PreDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon);

		//描画
		void Draw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon,
			ID3D12Device* device);

		//テクスチャ読み込み
		void LoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* device);

		//スプライト単体頂点バッファの転送
		void SpriteTransferVertexBuffer(const Sprite& sprite, uint32_t texIndex = UINT32_MAX);

	public:

		// 色設定
		void SetColor(Sprite sprite, const Vector4& color);

		// 透過度を設定
		void SetAlpha(Sprite sprite, float alpha_);

		//座標setter
		void SetPosition(const Vector3& position) { this->position_ = position; }
		//座標getter
		const Vector3& GetPosition() const { return position_; }
		//回転setter
		void SetRotation(const Vector3& rotation) { rotation_ = rotation; }
		//回転getter
		const Vector3& GetRotation() const { return rotation_; }
		//scaleGetter
		Vector2 GetScale() { return scale_; }
		//scaleSetter
		void SetScale(Vector2 scale) { scale_ = Vector2(scale.x, scale.y); }
		//色getter
		const Vector4& GetColor() const { return color_; }
		//表示サイズsetter
		void SetSize(const Vector2& size) { texSize_ = size; }
		//表示サイズgetter
		const Vector2& GetSize() const { return texSize_; }
		//アンカーポイントsetter
		void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
		//アンカーポイントgetter
		const Vector2& GetAnchorPoint() const { return anchorPoint_; }
		//左右フリップsetter
		bool SetFripX(bool isFlipX) { isFlipX_ = isFlipX; }
		//左右フリップgetter
		bool GetFlipX() const { return isFlipX_; }
		//上下フリップsetter
		bool SetFripY(bool isFlipY) { isFlipY_ = isFlipY; }
		//上下フリップgetter
		bool GetFlipY() const { return isFlipY_; }
		//非表示フラグsetter
		bool SetInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
		//非表示フラグgetter
		bool GetInvisible() const { return isInvisible_; }
		//テクスチャ番号setter
		void SetTextureIndex(UINT texNumber) { texNumber = texNumber_; }
		//テクスチャ番号getter
		UINT GetTextureIndex() const { return texNumber_; }

	private:
		//頂点バッファ
		ComPtr<ID3D12Resource> vertBuff_;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView_{};
		D3D12_RESOURCE_DESC resDesc_ = CD3DX12_RESOURCE_DESC();

	private:
		//DirectXCommonのインスタンス
		SIEngine::DirectXCommon* dxCommon_;
		//SpriteCommonのインスタンス
		SpriteCommon spriteCommon_;
		//コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> cmdList_;
		//デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> descHeap_;
		//SRVの最大個数
		static const size_t kMaxSRVCount_ = 2056;
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffMaterial_;
		// マッピング済みアドレス
		ConstBufferDataB0* constMap_ = nullptr;
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_ = {};
		//テクスチャバッファ
		array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;


		// テクスチャ番号
		UINT texNumber_ = 0;
		uint32_t texIndex_ = 0;

		//座標
		Vector3 position_ = { 0.0f,0.0f,0.0f };
		//スケール
		Vector2 scale_ = { 1,1 };
		//回転
		Vector3 rotation_ = { 0.0f,0.0f,0.0f };
		// ローカルワールド変換行列
		Matrix4 matWorld_;
		//色(RGBA)
		Vector4 color_ = { 1,1,1,1 };
		// テクスチャ左上座標設定
		Vector2 texLeftTop_ = { 50.0f, 50.0f };
		// テクスチャ切り出しサイズ
		Vector2 texSize_ = { 100.0f, 100.0f };
		//アンカーポイント
		Vector2 anchorPoint_ = { 0.0f,0.0f };

		// Z軸回りの回転
		float rotationZ_ = 0.0f;
		//左右フリップ
		bool isFlipX_ = false;
		//上下フリップ
		bool isFlipY_ = false;
		//非表示フラグ
		bool isInvisible_ = false;
	};

}