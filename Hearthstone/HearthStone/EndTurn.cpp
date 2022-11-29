#include "stdafx.h"
#include "EndTurn.h"
#include "Hero.h"
#include "InputMgr.h"


EndTurn::EndTurn()
{
	ImageX = 80;
	ImageY = 52;
	SizeX = 50;
	SizeY = 25;
	x = 1040;
	y = 324;
}


EndTurn::~EndTurn()
{
}


void EndTurn::Update()
{
	if (Hero::MyTurnFlag == true)
	{
		RENDINST.ShowImage(x - ImageX, y - ImageY, L"endturn.bmp");
	}

	if (INPUTINST.ButtonTest(this) // 내 턴 종료
		&& Hero::MyTurnFlag == true
		&& Hero::PrevFlag == true
		&& Hero::mulligan == false
		&& Hero::victory == 0)
	{
		Hero::MyTurnFlag = false;
		ResMgr::Inst().PlaySound(L"endturn.mp3");
	}
}