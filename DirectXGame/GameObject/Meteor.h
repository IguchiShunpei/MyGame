#pragma once

#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"

//GameScene‚Ì‘O•ûéŒ¾
class GamePlayScene;

class Meteor : public Object3d
{
public:
	//‰Šú‰»
	void MeteorInitialize();

	//XV
	void MeteorUpdate();

	//ˆÚ“®
	void Move();

	void SetMeteorPosition(const Vector3& position) { this->position = position; }
private:
	Vector3 position;
};