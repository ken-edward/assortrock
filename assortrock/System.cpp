
#include "System.hpp"

#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

int main() {

    return System::Run();

} // int main()

/* GLOBAL VARIABLES */
namespace System {

MSG g_msg = {};

HINSTANCE g_hinstance = {};
WNDCLASSEX g_wcex = {};
HWND g_hwnd = {};
RECT g_rect = {};

} // namespace System

/* RUN */
namespace System {
    
int Run() {

    #ifdef _DEBUG
    _CrtSetDbgFlag(
        _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF
    );
    #endif // _DEBUG

    if (Init()) { Loop(); } Quit();

    return (int)g_msg.wParam;

} // int Run()

} // namespace System

/* CORE */
namespace System {

bool Init() {
    
    if (!Window_Init()) { return false; }

    return true;

} // bool Init()

void Quit() {

    Window_Quit();

} // void Quit()

void Loop() {

    while (g_msg.message != WM_QUIT) {
    
        if (PeekMessage(&g_msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&g_msg);
            DispatchMessage(&g_msg);
        } else {
            Update();
        }
    
    }

} // void Loop()

} // namespace System

/* WNDPROC */
namespace System {

LRESULT __stdcall WndProc(
    HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam
) {
    
    switch (umsg) {
        case WM_SIZE: {
            if (g_hwnd) {
                GetClientRect(hwnd, &g_rect);
            }
        } break;
        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        default: {
            return DefWindowProc(hwnd, umsg, wparam, lparam);
        } break;
    }

    return 0;

} // LRESULT __stdcall WndProc

} // namespace System

/* UPDATE */
namespace System {

void Update() {

    

} // void Update()

}

/* WINDOW */
namespace System {

bool Window_Init() {
    g_wcex.cbSize = sizeof(WNDCLASSEX);
    g_wcex.style = CS_HREDRAW | CS_VREDRAW;
    g_wcex.lpfnWndProc = WndProc;
    g_wcex.cbClsExtra = 0;
    g_wcex.cbWndExtra = 0;
    g_wcex.hInstance = g_hinstance;
    g_wcex.hIcon = NULL;
    g_wcex.hCursor = NULL;
    g_wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    g_wcex.lpszMenuName = NULL;
    g_wcex.lpszClassName = TEXT("assortrock");
    g_wcex.hIconSm = 0;
    RegisterClassEx(&g_wcex);

    g_hwnd = CreateWindowEx(
        0, g_wcex.lpszClassName, TEXT("assortrock"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, g_wcex.hInstance, NULL
    );

    if (g_hwnd == NULL) {
        return false;
    } else {
        ShowWindow(g_hwnd, SW_SHOW);
    }

    return true;
}

void Window_Quit() {
    UnregisterClass(g_wcex.lpszClassName, g_wcex.hInstance);
}

}
