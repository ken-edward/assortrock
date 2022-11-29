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
private: // 멤버 변수
#define MAX_LOADSTRING 100
	HINSTANCE m_hInst;								// 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
	static bool m_CoreLive;
	HDC m_HDC;
	HDC m_backMemDC;
	HBITMAP m_backBMP;
	HWND m_hWnd;
	RECT m_WndSize;
	State* m_pCurState;
	std::vector<State*> m_AllState;

public: // 변수 반환
	HINSTANCE HINST() { return m_hInst;	}
	HDC	BackDC() { return m_backMemDC; }
	HBITMAP BackBMP() { return m_backBMP; }
	HWND MHWND() { return m_hWnd; }
	RECT WNDSIZE() { return m_WndSize; }

public: // 싱글톤
	static APICORE& Inst()
	{
		static APICORE CoreInst = APICORE();
		return CoreInst;
	}

public: // Start 함수
	int APIENTRY Start(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);

private: // 멤버 함수
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