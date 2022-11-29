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

			if (CheckKey == 1) // Ű�� ���ȴµ�
			{
				if (m_bUpState == true) // Ű�� �� ������ �־��ٸ�
				{
					m_bUp = false;
					m_bUpState = false;
					m_bDown = true; // ���� �� ���� ���Դϴ�.
					m_bDownState = true;
				}
				else // Ű�� ������ �־��ٸ�
				{
					m_bUp = false;
					m_bUpState = false;
					m_bDown = false; 
					m_bDownState = true; // ��� ������ �ִ� ���Դϴ�.
				}
			}
			else // Ű�� �� ���ȴµ�
			{
				if (m_bUpState == true) // Ű�� �� ������ �־��ٸ�
				{
					m_bUp = false;
					m_bUpState = true; // ��� ���� �ִ� ���Դϴ�.
					m_bDown = false;
					m_bDownState = false;
				}
				else // Ű�� ������ �־��ٸ�
				{
					m_bUp = true; // ���� �� �� ���Դϴ�.
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
	}; // ��������� InputData Ŭ�����Դϴ�.

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
