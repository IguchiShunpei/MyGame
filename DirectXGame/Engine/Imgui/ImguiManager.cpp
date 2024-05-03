/**
* @file ImguiManager.cpp
* @brief Imguiの処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#include "ImguiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <cassert>

SIEngine::ImGuiManager::ImGuiManager()
{
}

SIEngine::ImGuiManager::~ImGuiManager()
{
}

SIEngine::ImGuiManager* SIEngine::ImGuiManager::GetInstance()
{
	static SIEngine::ImGuiManager instance;

	return &instance;
}

void SIEngine::ImGuiManager::Initialize(SIEngine::DirectXCommon* dxCommon, SIEngine::WinApp* winApp)
{
	HRESULT result;

	//imguiのコンテキストを生成
	ImGui::CreateContext();
	//imguiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHwnd());

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//デスクリプタヒープ生成
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));

	//DirectX用初期化
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		static_cast<int32_t>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO io = ImGui::GetIO();
	//標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void SIEngine::ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタヒープ解放
	srvHeap_.Reset();
}

void SIEngine::ImGuiManager::Begin()
{
	//imguiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void SIEngine::ImGuiManager::End()
{
	//描画前準備
	ImGui::Render();
}

void SIEngine::ImGuiManager::Draw(SIEngine::DirectXCommon* dxCommon)
{
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
