#include "Window.h"
#include <TexCom.h>
#include <ColCom.h>
#include <ColMgr.h>
#include <SoundClass.h>
#include <Scene.h>
#include "SoundRing.h"


Window::Window(VEC2 leftBot, VEC2 _size, bool dir)
{
	if (dir == 0) // left facing window
	{
		tex = (TexCom*)AddCom(new TexCom(L"../tex/leftwindowframe.png"));
	}
	else // right facing window
	{
		tex = (TexCom*)AddCom(new TexCom(L"../tex/rightwindowframe.png"));
	}

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"window", col);

	frame = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"frame", frame);

	glass = (TexCom*)AddCom(new TexCom(L"../tex/window.png"));

	tex->SetZ(5.0f);
	glass->SetZ(3.0f);
	
	VEC2 midpos = leftBot + _size / 2.0f;

	SetPos(midpos);
	SetSize(_size / 2.0f);

	if (dir == 0)
	{
		glass->Move({ -size.x * 0.5f, 0.0f });
		glass->SetSize({ size.x * 0.5f, size.y });
	}
	else
	{
		glass->Move({ size.x * 0.5f, 0.0f });
		glass->SetSize({ size.x * 0.5f, size.y });
	}
}


Window::~Window()
{
}


void Window::Hit(VEC2 hitpoint)
{
	hp -= 30;

	if (hp <= 0)
	{
		Break();
	}
	else
	{
		SoundClass::Inst().Play(L"glasshit");
		Scene::GetCurScene()->AddActor(new SoundRing(hitpoint, 300.0f));
	}
}


void Window::Break()
{
	SoundClass::Inst().Play(L"glassbreak");
	col->bAlive = false;
	glass->bAlive = false;
	Scene::GetCurScene()->AddActor(new SoundRing( pos, 800.0f ));
}