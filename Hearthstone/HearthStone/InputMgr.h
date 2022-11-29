#pragma once
#include <string>
#include <map>
#include "APICORE.h"

#define INPUTINST InputMgr::Inst()

class Actor;
class InputMgr
{
	class InputData
	{
		friend InputMgr;

	private:
		bool m_bUp;
		bool m_bUpState;
		bool m_bDown;
		bool m_bDownState;
		char m_Key;

	public:
		bool IsUp()
		{
			return m_bUp;
		}
		bool IsUpState()
		{
			return m_bUpState;
		}
		bool IsDown()
		{
			return m_bDown;

		}
		bool IsDownState()
		{
			return m_bDownState;
		}

	public:
		void Update()
		{
			bool CheckKey = 0;

			if (GetAsyncKeyState(m_Key))
			{
				CheckKey = 1;
			}

			if (CheckKey == 1) // 키가 눌렸는데
			{
				if (m_bUpState == true) // 키를 안 누르고 있었다면
				{
					m_bUp = false;
					m_bUpState = false;
					m_bDown = true; // 이제 막 누른 것입니다.
					m_bDownState = true;
				}
				else // 키를 누르고 있었다면
				{
					m_bUp = false;
					m_bUpState = false;
					m_bDown = false; 
					m_bDownState = true; // 계속 누르고 있던 것입니다.
				}
			}
			else // 키가 안 눌렸는데
			{
				if (m_bUpState == true) // 키를 안 누르고 있었다면
				{
					m_bUp = false;
					m_bUpState = true; // 계속 떼고 있던 것입니다.
					m_bDown = false;
					m_bDownState = false;
				}
				else // 키를 누르고 있었다면
				{
					m_bUp = true; // 이제 막 뗀 것입니다.
					m_bUpState = true;
					m_bDown = false;
					m_bDownState = false;
				}
			}
		}

	public:
		InputData()
			: m_bUp(false)
			, m_bUpState(true)
			, m_bDown(false)
			, m_bDownState(false)
		{
		}
		~InputData()
		{
		}
	}; // 여기까지가 InputData 클래스입니다.

public:
	static InputMgr& Inst()
	{
		static InputMgr InputInst = InputMgr();
		return InputInst;
	}

private:
	std::map<std::string, InputData*> m_AllKeyMap;
	POINT m_Cursor;

public:
	void CreateKey(std::string _KeyName, char _Command)
	{
		InputData* newKey = new InputData;
		newKey->m_Key = _Command;
		m_AllKeyMap.insert(std::pair<std::string, InputData*>(_KeyName, newKey));
	}

	InputData* FindKey(std::string _KeyName)
	{
		return m_AllKeyMap.find(_KeyName)->second;
	}

	POINT Cursor()
	{
		return m_Cursor;
	}

public:
	void Update()
	{
		std::map<std::string, InputData*>::iterator StartIter;
		std::map<std::string, InputData*>::iterator EndIter;
		StartIter = m_AllKeyMap.begin();
		EndIter = m_AllKeyMap.end();

		for (; StartIter != EndIter; StartIter++)
		{
			StartIter->second->Update();
		}

		GetCursorPos(&m_Cursor);
		ScreenToClient(APICORE::Inst().MHWND(), &m_Cursor);
	}

public:
	void Init();
	bool CursorTest(Actor* _Actor);
	bool CursorTestByRect(int left, int top, int right, int bottom);
	bool ButtonTest(Actor* _Actor);
	bool ButtonTestByRect(int left, int top, int right, int bottom);

private:
	InputMgr();
	~InputMgr();
};
