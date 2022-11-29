#include "stdafx.h"
#include "SystemClass.h"
#include "DXClass.h"
#include "Scene.h"
#include "GameCore.h"
#include "TimeClass.h"
#include "InputClass.h"
#include "SoundClass.h"
#include "Cam.h"
#include "RenderMgr.h"
#include "FontWrapper.h"


SystemClass::SystemClass()
{
}


SystemClass::~SystemClass()
{
}


void SystemClass::Init(GameCore* gamecore)
{
	InitializeWindow();
	DXClass::Inst().Init(m_hwnd, m_width, m_height);
	InputClass::Inst().Init();
	SoundClass::Inst().Init();
	FontWrapper::Inst().Init();
	gamecore->Init();
}


void SystemClass::Run()
{
	// 메시지 구조체 생성 및 초기화
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 사용자로부터 종료 메시지를 받을때까지 메시지루프를 돕니다.
	while (corelive && msg.message != WM_QUIT)
	{
		// 윈도우 메시지를 처리합니다
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 그 외에는 Frame 함수를 처리합니다.
			Frame();
		}
	}
}


void SystemClass::End()
{
	Scene::Shutdown();
	ReleaseWindow();
	DXClass::Inst().End();
}


void SystemClass::InitializeWindow()
{
	// 이 프로그램의 인스턴스를 가져옵니다.
	m_hinst = GetModuleHandle(NULL);

	// 프로그램 이름을 지정합니다
	m_appName = L"DirectX";

	// windows 클래스를 아래와 같이 설정합니다.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// windows class를 등록합니다.
	RegisterClassEx(&wc);
	
	// 윈도우 크기를 지정합니다. stdafx.h 참조.
	m_width = WIDTH;
	m_height = HEIGHT;

	// 윈도우 창을 가로, 세로의 정 가운데 오도록 합니다.
	int posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	// 윈도우를 생성하고 핸들을 구합니다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_width, m_height, NULL, NULL, m_hinst, NULL);

	// 윈도우를 화면에 표시하고 포커스를 지정합니다.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}


LRESULT CALLBACK SystemClass::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	// 윈도우 종료를 확인합니다.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// 윈도우가 닫히는지 확인합니다.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	// ESC가 눌렸는지 확인합니다.
	case WM_KEYDOWN:
		if ((int)wparam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
	// 그 외의 모든 메시지들은 기본 메시지 처리로 넘깁니다.
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}


void SystemClass::Frame()
{
	TimeClass::Inst().Update();
	InputClass::Inst().Update();
	SoundClass::Inst().Update();

	DXClass::Inst().Clear();
	Scene::SceneUpdate();
	DXClass::Inst().Present();
}


void SystemClass::ReleaseWindow()
{
	// 창을 제거합니다
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 프로그램 인스턴스를 제거합니다
	UnregisterClass(m_appName, m_hinst);
	m_hinst = NULL;
}