#define DIRECTINPUT_VERSION    0x0800 //DirectInput�̃o�[�W�����w��
#include "GameScene.h"
#include "SIFrameWork.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SIFrameWork* game = new GameScene();
	
	game->Run();

	delete game;

	return 0;
}
