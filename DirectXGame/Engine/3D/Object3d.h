/**
* @file Object3d.h
* @brief  3Dオブジェクトに関するクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "CollisionInfo.h"
#include <memory>

class BaseCollider;

/// 3Dオブジェクト
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 静的メンバ関数
	/// 静的初期化
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	static void PreLineDraw(ID3D12GraphicsCommandList* cmdList);

	/// 描画後処理
	static void PostDraw();

	/// 3Dオブジェクト生成
	static std::unique_ptr<Object3d> Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> lineRootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> linePipelinestate;

private:// 静的メンバ関数

	/// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

public: // メンバ関数

	//コンストラクタ
	Object3d() = default;

	//デストラクタ
	virtual ~Object3d();

	//初期化
	virtual bool Initialize();

	/// 毎フレーム処理
	virtual void Update();

	/// 描画
	void Draw(ViewProjection* viewProjection);
	void Draw(ViewProjection* viewProjection, float alpha_,Vector3 color_);

	//コライダーのセット
	void SetCollider(BaseCollider* collider);

	//衝突時コールバック関数
	virtual void OnCollision([[maybe_unused]] const CollisionInfo& info) {}
	virtual void OffCollision([[maybe_unused]] const CollisionInfo& info) {}

	// モデルの設定
	void SetModel(Model* model) { this->model_ = model; }

	// オブジェクトの座標
	const Vector3& GetPosition() const { return worldTransform_.position_; }
	const float& GetPositionZ() const { return worldTransform_.position_.z; }
	void SetPosition(const Vector3& position) { this->worldTransform_.position_ = position; }
	// オブジェクトの大きさ
	void SetScale(const Vector3& scale) { this->worldTransform_.scale_ = scale; }
	const Vector3& GetScale() const { return worldTransform_.scale_; }
	// オブジェクトの回転
	void SetRotation(const Vector3& rotation) { this->worldTransform_.rotation_ = rotation; }
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotationX(const float& rotation) { this->worldTransform_.rotation_.x = rotation; }
	void SetRotationY(const float& rotation) { this->worldTransform_.rotation_.y = rotation; }

	const char* GetName() const { return name; }
public:
	// ワールド変換データ
	WorldTransform worldTransform_;
	const char* toCollisionName = nullptr;

protected: // メンバ変数
	//クラス名
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;
	// モデル
	Model* model_ = nullptr;

};