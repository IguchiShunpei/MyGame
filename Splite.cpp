#include "Splite.h"

//���_�f�[�^
XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},//����
	{-0.5f,+0.5f,0.0f},//����
	{+0.5f,-0.5f,0.0f},//�E��
};

//���_�f�[�^�̑S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

void Splite::Initialize(SpliteCommon* spliteCommon)
{
	//�`��R�}���h
	spliteCommon->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);  //�S�Ă̒��_���g���ĕ`��
}