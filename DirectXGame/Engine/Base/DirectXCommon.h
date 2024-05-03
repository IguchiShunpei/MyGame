/**
* @file DirectXCommon.h
* @brief DirectXの基盤クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include <cstdlib>
#include<chrono>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include "FPSFixed.h"

namespace SIEngine
{
	class DirectXCommon
	{
	public:
		static DirectXCommon* GetInstance();

	private:
		//コンストラクタ
		DirectXCommon() = default;
		//デストラクタ
		~DirectXCommon() = default;
		//コピー封印
		DirectXCommon(const DirectXCommon&) = delete;
		//コピー封印
		DirectXCommon& operator=(const DirectXCommon&) = delete;

	public:
		//初期化
		void Initialize(WinApp* winApp);

		//デバイスの初期化
		void InitializeDevice();

		//コマンド関連の初期化
		void InitializeCommand();

		//スワップチェーンの初期化
		void InitializeSwapChain();

		//レンダーターゲットビューの初期化
		void InitializeRendeTargetView();

		//深度バッファビューの初期化
		void InitializeDepthBuffer();

		//フェンスの初期化
		void InitializeFence();

		//描画前処理
		void PreDraw();

		//描画後処理
		void PostDraw();

		//デバイス取得
		ID3D12Device* GetDevice() const { return device_.Get(); }

		//コマンドリスト取得
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

		// 終了処理
		void fpsFixedFinalize();

		//バックバッファの数を取得
		size_t GetBackBufferCount() const { return backBuffers_.size(); }

	private: //メンバ関数
		template <class T>
		inline void safe_delete(T*& p) {
			delete p;
			p = nullptr;
		}

	private:
		//WindowsAPI
		WinApp* winApp = nullptr;
		//FPS
		FPSFixed* fpsFixed = nullptr;

		//DXGIファクトリ
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
		//DirectX12デバイス
		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		//コマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
		//コマンドアロケータ
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator_;
		//スワップチェーン
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
		//コマンドキュー
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
		//バックバッファ
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
		//深度バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;
		//レンダーターゲットビュー
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		//深度バッファビュー
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;
		// リソースバリア
		D3D12_RESOURCE_BARRIER barrierDesc_;
		//フェンス
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		UINT64 fenceVal_ = 0;
		// 結果確認
		HRESULT result;
	};
}