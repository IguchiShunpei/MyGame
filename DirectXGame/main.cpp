/**
* @file main.cpp
* @brief  メインのクラス
* @author イグチ_シュンペイ
*/

#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SIFrameWork* game = new Game();

	game->Run();

	delete game;

	return 0;
}