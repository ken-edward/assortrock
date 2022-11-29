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
	// �޽��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ����ڷκ��� ���� �޽����� ���������� �޽��������� ���ϴ�.
	while (corelive && msg.message != WM_QUIT)
	{
		// ������ �޽����� ó���մϴ�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó���մϴ�.
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
	// �� ���α׷��� �ν��Ͻ��� �����ɴϴ�.
	m_hinst = GetModuleHandle(NULL);

	// ���α׷� �̸��� �����մϴ�
	m_appName = L"DirectX";

	// windows Ŭ������ �Ʒ��� ���� �����մϴ�.
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

	// windows class�� ����մϴ�.
	RegisterClassEx(&wc);
	
	// ������ ũ�⸦ �����մϴ�. stdafx.h ����.
	m_width = WIDTH;
	m_height = HEIGHT;

	// ������ â�� ����, ������ �� ��� ������ �մϴ�.
	int posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	// �����츦 �����ϰ� �ڵ��� ���մϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_width, m_height, NULL, NULL, m_hinst, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �����մϴ�.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}


LRESULT CALLBACK SystemClass::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	// ������ ���Ḧ Ȯ���մϴ�.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// �����찡 �������� Ȯ���մϴ�.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	// ESC�� ���ȴ��� Ȯ���մϴ�.
	case WM_KEYDOWN:
		if ((int)wparam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
	// �� ���� ��� �޽������� �⺻ �޽��� ó���� �ѱ�ϴ�.
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
	// â�� �����մϴ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷� �ν��Ͻ��� �����մϴ�
	UnregisterClass(m_appName, m_hinst);
	m_hinst = NULL;
}