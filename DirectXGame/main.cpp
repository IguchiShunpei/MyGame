#define DIRECTINPUT_VERSION    0x0800 //DirectInput�̃o�[�W�����w��
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SIFrameWork* game = new Game();

	game->Run();

	delete game;

	return 0;
}