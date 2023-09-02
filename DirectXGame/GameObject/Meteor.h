#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"

//GameScene�̑O���錾
class GamePlayScene;

class Meteor : public Object3d
{
public:
	//������
	void MeteorInitialize();

	//�X�V
	void MeteorUpdate();

	//�ړ�
	void Move();

	void SetMeteorPosition(const Vector3& position) { this->position = position; }
private:
	Vector3 position;
};