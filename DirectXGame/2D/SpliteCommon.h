#include "DirectXCommon.h"
#include "wrl.h"
#include <DirectXMath.h>

using namespace DirectX;

#pragma once
class SpliteCommon
{

public:
	//namespace�ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//������
	void Initialize(DirectXCommon*dxCommon);

	//getter
	DirectXCommon* GetDxCommon() const { return dxCommon; }

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device*device;

private://�����o�ϐ�
	// DirectXCommon�̃C���X�^���X
	DirectXCommon* dxCommon = nullptr;
};

