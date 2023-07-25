#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include<windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include<fbxsdk.h>

using namespace DirectX;

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	Vector4 scaling = { 1,1,1,0 };
	//���[�J����]�p
	Vector4 rotation = { 0,0,0,0 };
	//���[�J���ړ�
	Vector4 translation = { 0,0,0,0 };
	//���[�J���ό`�s��
	Matrix4 transform;
	//�O���[�o���ό`�s��
	Matrix4 glabalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://�萔
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public://�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		Vector3 pos; //xyz���W
		Vector3 normal; //�@���x�N�g��
		Vector2 uv; //uv���W
		UINT boneIndex[MAX_BONE_INDICES];//�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES];//�{�[���d��
	};

	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		Matrix4 invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
public:
	//�t�����h�N���X
	friend class FbxLoader;
public:
	//�f�X�g���N�^
	~FbxModel();
	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//���f���̕ό`�s��擾(�萔�o�b�t�@�Ƀ��[���h�s��𑗂邽��)
	const Matrix4& getModelTransform() { return meshNode->glabalTransform; }
	// setter
	static void SetDevice(ID3D12Device* device) { FbxModel::device = device; }
	//getter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
private:
	// �f�o�C�X
	static ID3D12Device* device;
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//�{�[���z��
	std::vector<Bone> bones;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�A���r�G���g�W��
	Vector3 ambient = { 1,1,1 };
	Vector3 diffuse = { 1,1,1 };
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	//���_�o�b�t�@
	ComPtr<ID3D12Resource>vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource>indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
};
