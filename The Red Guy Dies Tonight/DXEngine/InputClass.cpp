#include "stdafx.h"
#include "InputClass.h"
#include "DXClass.h"
#include "Cam.h"


void KeyData::Update()
{
	bool isPressed = false;

	
	bool isConsoleWindowFocussed = (DXClass::Inst().GetHWND() == GetForegroundWindow());
	if (isConsoleWindowFocussed)
	{
		if (GetAsyncKeyState(m_key))
		{
			isPressed = true;
		}
	}

	if (isPressed) // 이번에 눌렸다면
	{
		// 뗀 것 관련 false
		m_justUp = false;
		m_holdUp = false;
		
		// 막 눌렸는지 확인
		if (m_holdDn)
		{
			m_justDn = false;
		}
		else
		{
			m_justDn = true;
		}

		// 눌린 상태 자체 true
		m_holdDn = true;
	}
	else // 이번에 눌린 게 아니면
	{
		// 누른 것 관련 false
		m_justDn = false;
		m_holdDn = false;

		// 막 뗐는지 확인
		if (m_holdUp)
		{
			m_justUp = false;
		}
		else
		{
			m_justUp = true;
		}

		// 뗀 상태 자체 true
		m_holdUp = true;
	}
}


InputClass::InputClass()
{
}

InputClass::~InputClass()
{
	auto iter = keyMap.begin();
	auto end = keyMap.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}

	keyMap.clear();
}



void InputClass::Init()
{
	AddKey('1');
	AddKey('2');
	AddKey('3');
	AddKey('4');

	AddKey('Q');
	AddKey('W');
	AddKey('E');
	AddKey('R');

	AddKey('A');
	AddKey('S');
	AddKey('D');
	AddKey('F');

	AddKey('Z');
	AddKey('X');
	AddKey('C');
	AddKey('V');

	AddKey('T');
	AddKey('G');
	AddKey('B');

	AddKey('U');
	AddKey('I');
	AddKey('O');
	AddKey('P');

	CreateKey(L"Up", VK_UP);
	CreateKey(L"Down", VK_DOWN);
	CreateKey(L"Left", VK_LEFT);
	CreateKey(L"Right", VK_RIGHT);

	CreateKey(L"Click", VK_LBUTTON);
	CreateKey(L"RClick", VK_RBUTTON);

	CreateKey(L"Space", VK_SPACE);
	CreateKey(L"Shift", VK_SHIFT);
	CreateKey(L"ESC", VK_ESCAPE);
}


void InputClass::Update()
{
	auto iter = keyMap.begin();
	auto end = keyMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update();
	}


	GetCursorPos(&Cursor);
	ScreenToClient(DXClass::Inst().GetHWND(), &Cursor);
	
	Cursor.y = HEIGHT - Cursor.y; // 위에서부터 측정을 아래서부터로 변경

	Cursor.x -= WIDTH / 2;
	Cursor.y -= HEIGHT / 2; // DX 기준으로 변경
}


void InputClass::AddKey(int key)
{
	wchar_t arr[2] = {};

	arr[0] = key;
	
	CreateKey(arr, key);
}


void InputClass::CreateKey(std::wstring keyname, int key)
{
	KeyData* newData = new KeyData;
	
	newData->m_key = key;

	keyMap.insert(pair<wstring, KeyData*>(keyname, newData));
}


KeyData* InputClass::FindKey(std::wstring keyname)
{
	auto iter = keyMap.find(keyname);

	if (iter == keyMap.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}


VEC2 InputClass::IngameCursor()
{
	VEC2 P;
	P.x = (float)Cursor.x;
	P.y = (float)Cursor.y;

	P *= Cam::Inst().GetSize();

	P += Cam::Inst().GetPos();

	return P;
}