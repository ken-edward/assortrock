#include "stdafx.h"
#include "Actor.h"


Actor::Actor()
	: x(0.0f), y(0.0f)
	, ImageX(0.0f), ImageY(0.0f)
	, SizeX(0.0f), SizeY(0.0f), scale(1.0f)
	, m_bDeath(false)
	, Name(L"None")
{
}


Actor::~Actor()
{
}
