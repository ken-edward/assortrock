#include "stdafx.h"
#include "SelectState.h"
#include "RenderMgr.h"
#include "InputMgr.h"


SelectState::SelectState()
{
}


SelectState::~SelectState()
{
}


void SelectState::StateUpdate()
{
	RENDINST.ShowImage(0, 0, L"SelectState.bmp");

	if (INPUTINST.ButtonTestByRect(870, 530, 1000, 650))
	{
		SetNextState(STATE::PLAY);
	}

	if (INPUTINST.ButtonTestByRect(1020, 646, 1090, 682))
	{
		SetNextState(STATE::MENU);
	}

	/*
	wchar_t str[100]{};
	swprintf_s(str, L"MousePosX: %d", INPUTINST.Cursor().x);
	RENDINST.ShowText(6, 15, str);
	swprintf_s(str, L"MousePosY: %d", INPUTINST.Cursor().y);
	RENDINST.ShowText(6, 35, str);
	*/
}


void SelectState::Start()
{
	bgm->Stop();
}