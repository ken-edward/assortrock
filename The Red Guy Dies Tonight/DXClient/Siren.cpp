#include "Siren.h"
#include "NPC.h"
#include "MapMgr.h"


Siren::Siren(VEC2 midBot)
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/alarm.png"));
	triggered = (TexCom*)AddCom(new TexCom(L"../tex/alarm_triggered.png"));
	col = (ColCom*)AddCom(new ColCom);
	col->SetSize({ 30.0f, 50.0f });
	ColMgr::Inst().Add(L"siren", col);
	SetPos({ midBot.x, midBot.y + 200.0f });
	float z = 8.0f;
	tex->SetZ(z);
	triggered->SetZ(z);
	float scale = 4.0f;
	tex->SetScale(scale);
	triggered->SetScale(scale);
	MapMgr::Inst().RegisterSiren(this);
}


Siren::~Siren()
{
}


void Siren::Update2()
{
	if (NPC::alarmed == false)
	{
		tex->display = true;
		triggered->display = false;
	}
	else
	{
		tex->display = false;
		triggered->display = true;
	}
}