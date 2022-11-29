#include "TitleActor.h"
#include <TexCom.h>
#include <RenderMgr.h>
#include <InputClass.h>
#include <Scene.h>
#include <TimeClass.h>
#include <SystemClass.h>


TitleActor::TitleActor()
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/title.png"));
	tex->SetPos({ 0, 150 });
	tex->SetScale(0.4f);
}


TitleActor::~TitleActor()
{
}


void TitleActor::Update2()
{
	RenderMgr::Inst().Font(L"Play", { WIDTH / 2 - 70, HEIGHT / 2 + 120 }, 60, { 0.0f, 0.0f, 0.0f, 1.0f });
	RenderMgr::Inst().Font(L"Quit", { WIDTH / 2 - 50, HEIGHT / 2 + 270 }, 33, { 0.0f, 0.0f, 0.0f, 1.0f });

	if (selected == false)
	{
		if (ButtonTest({ 0, -150 }, { 140, 30 }) || IsJustDn(L"Space"))
		{
			selected = true;
			task = 0;
		}
		if (ButtonTest({ 0, -286 }, { 140, 30 }))
		{
			SystemClass::Inst().corelive = false;
		}
	}


	if (selected)
	{
		delay += DT;

		switch (task)
		{
		case 0: // Play
			RenderMgr::Inst().Font(L"Loading...", { WIDTH - 220, HEIGHT - 70 }, 33, { 0.0f, 0.0f, 0.0f, 1.0f });
			if (delay > 0.1f)
			{
				Scene::ChangeScene(L"PlayScene");
			}
		default:
			break;
		}
	}
	else
	{
		delay = 0.0f;
	}
}


bool TitleActor::ButtonTest(POINT pos, POINT size)
{
	if (IsJustDn(L"Click"))
	{
		POINT cursor = InputClass::Inst().GetCursor();

		if (cursor.x > pos.x - size.x && cursor.x < pos.x + size.x)
		{
			if (cursor.y > pos.y - size.y && cursor.y < pos.y + size.y)
			{
				return true;
			}
		}
	}

	return false;
}