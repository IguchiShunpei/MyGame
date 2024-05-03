/**
* @file ImguiManager.h
* @brief Imguiの処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"

#include <wrl.h>
#include <imgui.h>

namespace SIEngine
{
	class ImGuiManager
	{
	public:

		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		// コンストラクタ
		ImGuiManager();
		// デストラクタ
		~ImGuiManager();

	private:

		//SRV用デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> srvHeap_;
		//コピーコンストラクタ無効
		ImGuiManager(const ImGuiManager& obj) = delete;
		//代入演算子を無効
		ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	public:

		static ImGuiManager* GetInstance();


		void Initialize(SIEngine::DirectXCommon* dxCommon, SIEngine::WinApp* winApp);

		void Finalize();

		void Begin();

		void End();

		void Draw(SIEngine::DirectXCommon* dxCommon);
	};
}