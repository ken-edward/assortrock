#include <Windows.h>
#include <SystemClass.h>
#include <crtdbg.h>
#include <locale>
#include "GameClient.h"


int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	setlocale(LC_ALL, "");
	srand((unsigned int)time(NULL));

	SystemClass::Inst().Init(&GameClient::Inst());
	SystemClass::Inst().Run();
	SystemClass::Inst().End();

	return 0;
}