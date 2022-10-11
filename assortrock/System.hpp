
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
using namespace std;

#ifdef _UNICODE
#define winmain wWinMain
typedef wstring tstring;
#else
#define winmain WinMain
typedef string tstring;
#endif // _UNICODE

#define winmain_parameter \
_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ INT

#define main() \
__stdcall winmain(winmain_parameter)

namespace System {

int Run();

} // namespace System

namespace System {

bool Init();
void Quit();
void Loop();
LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();

} // namespace System

namespace System {

bool Window_Init();
void Window_Quit();

} // namespace System
