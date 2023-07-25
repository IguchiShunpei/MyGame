#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "FbxModel.h"
#include "FbxLoader.h"

/// 3D�I�u�W�F�N�g
class FbxObject3d
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public:
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONES];
	};


public: // �ÓI�����o�֐�
	/// �ÓI������
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// �`��㏈��
	static void PostDraw();

	/// 3D�I�u�W�F�N�g����
	static FbxObject3d* Create();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

private:// �ÓI�����o�֐�

	/// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();

public: // �����o�֐�

	//������
	bool Initialize();

	/// ���t���[������
	void Update();

	/// �`��
	void Draw(ViewProjection* viewProjection);

	//���[���h�s��̎擾
	const Matrix4& GetMatWorld() { return matWorld; }

	// ���f���̐ݒ�
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	// �I�u�W�F�N�g�̍��W
	const Vector3& GetPosition() const { return worldTransform_.position_; }
	const float& GetPositionZ() const { return worldTransform_.position_.z; }
	void SetPosition(const Vector3& position) { this->worldTransform_.position_ = position; }
	// �I�u�W�F�N�g�̑傫��
	void SetScale(const Vector3& scale) { this->worldTransform_.scale_ = scale; }
	const Vector3& GetScale() const { return worldTransform_.scale_; }
	// �I�u�W�F�N�g�̉�]
	void SetRotation(const Vector3& rotation) { this->worldTransform_.rotation_ = rotation; }
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotationX(const float& rotation) { this->worldTransform_.rotation_.x = rotation; }
	void SetRotationY(const float& rotation) { this->worldTransform_.rotation_.y = rotation; }

public:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

protected: // �����o�ϐ�
	//�N���X��
	const char* name = nullptr;
	// ���f��
	FbxModel* fbxModel = nullptr;
	// ���[�J�����[���h�ϊ��s��
	Matrix4 matWorld;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
};

