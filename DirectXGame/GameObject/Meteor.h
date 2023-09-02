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

private:
	Vector3 position;
};