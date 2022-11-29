#pragma once
#include <vector>
enum STATE
{
	MENU,
	SELECT,
	PLAY,
	DECK,
	MAX
};

class State;
class APICORE
{
private: // ��� ����
#define MAX_LOADSTRING 100
	HINSTANCE m_hInst;								// ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
	static bool m_CoreLive;
	HDC m_HDC;
	HDC m_backMemDC;
	HBITMAP m_backBMP;
	HWND m_hWnd;
	RECT m_WndSize;
	State* m_pCurState;
	std::vector<State*> m_AllState;

public: // ���� ��ȯ
	HINSTANCE HINST() { return m_hInst;	}
	HDC	BackDC() { return m_backMemDC; }
	HBITMAP BackBMP() { return m_backBMP; }
	HWND MHWND() { return m_hWnd; }
	RECT WNDSIZE() { return m_WndSize; }

public: // �̱���
	static APICORE& Inst()
	{
		static APICORE CoreInst = APICORE();
		return CoreInst;
	}

public: // Start �Լ�
	int APIENTRY Start(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);

private: // ��� �Լ�
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	void StateUpdate();
	void Init();
	template<typename T>
	void CreateState(STATE _State)
	{
		m_AllState[_State] = new T;
	}

private:
	APICORE();
	~APICORE();
};