// AR16_CPY.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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