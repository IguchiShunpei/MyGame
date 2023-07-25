#include "SkyDome.h"

SkyDome::~SkyDome()
{
	delete skyModel;
}

void SkyDome::SkyDomeInitialize()
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	skyModel = Model::LoadFromOBJ("skydome");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(skyModel);
	SetScale(Vector3(454.0f, 340.0f, 500.0f));
}
