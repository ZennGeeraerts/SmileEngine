#pragma once

#ifdef SM_PLATFORM_WINDOWS

#include <Windows.h>

extern Smile::SmileGame* Smile::CreateGame();

void main(int argc, char** argv)
{
	WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	Smile::SmileGame* pGame = Smile::CreateGame();
	pGame->Run();
	delete pGame;
	return 0;
}
#endif