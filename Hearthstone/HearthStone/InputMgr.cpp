#include "stdafx.h"
#include "InputMgr.h"
#include "APICORE.h"
#include "Actor.h"


InputMgr::InputMgr()
{
}


InputMgr::~InputMgr()
{
	std::map<std::string, InputData*>::iterator StartIter;
	std::map<std::string, InputData*>::iterator EndIter;
	StartIter = m_AllKeyMap.begin();
	EndIter = m_AllKeyMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		delete StartIter->second;
	}
}


void InputMgr::Init()
{
	InputMgr::Inst().CreateKey("Click", VK_LBUTTON);
	InputMgr::Inst().CreateKey("RClick", VK_RBUTTON);
	InputMgr::Inst().CreateKey("T", 'T');
}


bool InputMgr::CursorTest(Actor* _Actor)
{
	if (_Actor->x - _Actor->SizeX < m_Cursor.x && m_Cursor.x < _Actor->x + _Actor->SizeX &&
		_Actor->y - _Actor->SizeY < m_Cursor.y && m_Cursor.y < _Actor->y + _Actor->SizeY)
	{
		return true;
	}
	return false;
}


bool InputMgr::CursorTestByRect(int left, int top, int right, int bottom)
{
	if (left < m_Cursor.x && m_Cursor.x < right &&
		top < m_Cursor.y && m_Cursor.y < bottom)
	{
		return true;
	}
	return false;
}


bool InputMgr::ButtonTest(Actor* _Actor)
{
	if (INPUTINST.FindKey("Click")->IsDown() && CursorTest(_Actor))
	{
		return true;
	}
	return false;
}


bool InputMgr::ButtonTestByRect(int left, int top, int right, int bottom)
{
	if (INPUTINST.FindKey("Click")->IsDown() &&
		left < m_Cursor.x && m_Cursor.x < right &&
		top < m_Cursor.y && m_Cursor.y < bottom)
	{
		return true;
	}
	return false;
}