#define DIRECTINPUT_VERSION    0x0800 //DirectInputのバージョン指定
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SIFrameWork* game = new Game();

	game->Run();

	delete game;

	return 0;
}