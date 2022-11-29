#include "UI.h"
#include <RenderMgr.h>
#include "NPC.h"
#include <TimeClass.h>
#include "Player.h"


UI::UI()
{
}


UI::~UI()
{
}



void UI::Update5()
{
	if (NPC::redguydied == false)
	{
		if (Player::killmission)
		{
			RenderMgr::Inst().Font(L"¢Â  Kill the", { 50.0f, 50.0f }, 20);
			RenderMgr::Inst().Font(L"Red Guy", { 177.0f, 50.0f }, 20, { 1.0f, 0.0f, 0.0f, 1.0f });
		}
	}
	else
	{
		time += DT;
		if (time < 1.0f)
		{
			TimeClass::Inst().SlowMo(0.3f);
			RenderMgr::Inst().Font(L"¢Â  Mission Completed", { 50.0f, 50.0f }, 20);
		}
		else if (time < 1.5f)
		{
			TimeClass::Inst().SlowMo(1.0f);
			RenderMgr::Inst().Font(L"¢Â  Escape to Exit", { 50.0f, 50.0f }, 20);
		}
		else
		{
			RenderMgr::Inst().Font(L"¢Â  Escape to Exit", { 50.0f, 50.0f }, 20);
		}
	}

	RenderMgr::Inst().Font(L"1. AK47", { 50, HEIGHT - 120 });
	RenderMgr::Inst().Font(L"2. Silenced Pistol", { 50, HEIGHT - 90 });
	//RenderMgr::Inst().Font(L"3. Machete", { 50, HEIGHT - 60 });
}