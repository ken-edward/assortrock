// AR16_CPY.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HearthStone.h"
#include "APICORE.h"
#include <crtdbg.h>
#include <locale>
#include <stdlib.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	setlocale(LC_ALL, "Korean");
	srand((unsigned int)time(NULL));
	return APICORE::Inst().Start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}