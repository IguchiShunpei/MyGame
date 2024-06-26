/**
* @file Object3d.cpp
* @brief  3Dオブジェクトに関するクラス
* @author イグチ_シュンペイ
*/

#include "Object3d.h"
#include "BaseCollider.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "CollisionManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Object3d::device_ = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3d::rootsignature;
ComPtr<ID3D12PipelineState> Object3d::pipelinestate;
ComPtr<ID3D12RootSignature> Object3d::lineRootsignature;
ComPtr<ID3D12PipelineState> Object3d::linePipelinestate;

Object3d::~Object3d()
{
	if (collider)
	{
		//コリジョンマネージャから登録を解除する
		CollisionManager::GetInstance()->RemoveCollider(collider);

		delete collider;
	}
}

void Object3d::StaticInitialize(ID3D12Device* device, [[maybe_unused]] int window_width, [[maybe_unused]] int window_height)
{
	// nullptrチェック
	assert(device);

	Object3d::device_ = device;

	// モデルにデバイスをセット
	Model::SetDevice(device);

	// ワールドトランスフォームにデバイスを貸す
	WorldTransform::StaticInitialize(device);

	// パイプライン初期化
	InitializeGraphicsPipeline();
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList_)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3d::cmdList == nullptr);

	// コマンドリストをセット
	Object3d::cmdList = cmdList_;

	// パイプラインステートの設定
	cmdList_->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
//線描画
void Object3d::PreLineDraw(ID3D12GraphicsCommandList* cmdList_) {
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3d::cmdList == nullptr);

	// コマンドリストをセット
	Object3d::cmdList = cmdList_;

	// パイプラインステートの設定
	cmdList_->SetPipelineState(linePipelinestate.Get());

	// ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Object3d::PostDraw()
{
	// コマンドリストを解除
	Object3d::cmdList = nullptr;
}

std::unique_ptr<Object3d> Object3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		assert(0);
		return nullptr;
	}


	return object3d;
}

void Object3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&linePipelinestate));
	assert(SUCCEEDED(result));
}

bool Object3d::Initialize()
{
	//worldTransform初期化
	worldTransform_.Initialize();

	//クラス名の文字列を取得
	name = typeid(*this).name();

	return true;
}

void Object3d::Update()
{
	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();

	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}   
}

void Object3d::Draw(ViewProjection* viewProjection)
{
	// nullptrチェック
	assert(device_);
	assert(Object3d::cmdList);

	// モデルがセットされていなければ描画をスキップ
	if (model_ == nullptr) return;

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, worldTransform_.GetBuff()->GetGPUVirtualAddress());

	// ビュープロジェクション変換データ定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(1, viewProjection->GetBuff()->GetGPUVirtualAddress());

	// モデルを描画
	model_->Draw(cmdList, 2, 1.0f,Vector3(1.0f,1.0f,1.0f));
}

void Object3d::Draw(ViewProjection* viewProjection, float alpha_,Vector3 color_)
{
	// nullptrチェック
	assert(device_);
	assert(Object3d::cmdList);

	// モデルがセットされていなければ描画をスキップ
	if (model_ == nullptr) return;

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, worldTransform_.GetBuff()->GetGPUVirtualAddress());

	// ビュープロジェクション変換データ定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(1, viewProjection->GetBuff()->GetGPUVirtualAddress());

	// モデルを描画
	model_->Draw(cmdList, 2, alpha_,color_);
}

void Object3d::SetCollider(BaseCollider* collider_)
{
	collider_->SetObject(this);
	this->collider = collider_;
	//コリジョンマネージャに登録
	CollisionManager::GetInstance()->AddCollider(collider_);
	//コライダーを更新しておく
	collider_->Update();
}