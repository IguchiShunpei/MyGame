#pragma once 
#include "Object3d.h"

class SkyDome : public Object3d
{
public:
	//�f�X�g���N�^
	~SkyDome();
	//������
	void SkyDomeInitialize();

private:
	// ���f��
	Model* skyModel = nullptr;
};