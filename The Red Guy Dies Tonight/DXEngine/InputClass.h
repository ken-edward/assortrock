#pragma once
#include <unordered_map>
#include "Vector.h"

#define IsJustDn(P) InputClass::Inst().FindKey(P)->GetJustDn()
#define IsHoldDn(P) InputClass::Inst().FindKey(P)->GetHoldDn()
#define IsJustUp(P) InputClass::Inst().FindKey(P)->GetJustUp()
#define IsHoldUp(P) InputClass::Inst().FindKey(P)->GetHoldUp()

class KeyData
{
private:
	int m_key = 0;
	
private:
	bool m_justDn = false;
	bool m_holdDn = false;
	bool m_justUp = false;
	bool m_holdUp = true;

public:
	bool GetJustDn() { return m_justDn; }
	bool GetHoldDn() { return m_holdDn; }
	bool GetJustUp() { return m_justUp; }
	bool GetHoldUp() { return m_holdUp; }

private:
	void Update();

private:
	friend class InputClass;
};


class InputClass
{
private:
	InputClass();
	~InputClass();

public:
	static InputClass& Inst()
	{
		static InputClass Inst;
		return Inst;
	}

private:
	std::unordered_map<std::wstring, KeyData*> keyMap;

private:
	POINT Cursor;

public:
	void Init();
	void Update();

public:
	void AddKey(int);
	void CreateKey(std::wstring, int);
	KeyData* FindKey(std::wstring);
	POINT GetCursor() { return Cursor; }
	VEC2 IngameCursor();
};