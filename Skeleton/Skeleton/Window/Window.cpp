#include "Window.h"
#include "Icon.h"
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

// コンストラクタ　
Window::Window(const unsigned int & width, const unsigned int & height) :
	handle(nullptr), instance(nullptr), name(nullptr), width(width), height(height)
{
	Create();
}

// デストラクタ
Window::~Window()
{
	UnregisterClass(name, (HINSTANCE)(instance));
}

// タイトル名の変更
void Window::ChangeTitle(const std::tstring & title)
{
	auto hr = SetWindowText(reinterpret_cast<HWND>(handle), title.c_str());
	if (hr == 0)
	{
		OutputDebugString(_T("\nタイトル名の変更：失敗\n"));
	}
}

// ウィンドウのコールバック
#if _WIN64
long __stdcall Window::WindowProc(void * hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
#else
long __stdcall Window::WindowProc(void* hWnd, unsigned int message, long wParam, long lParam)
#endif
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return static_cast<long>(DefWindowProc(reinterpret_cast<HWND>(hWnd), message, wParam, lParam));
}

// ウィンドウの生成
void Window::Create(void)
{
	WNDCLASSEX wnd{};
	wnd.cbClsExtra    = 0;
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.cbWndExtra    = 0;
	wnd.hbrBackground = CreateSolidBrush(0x000000);
	wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon         = LoadIcon(nullptr, MAKEINTRESOURCE(ICON_ID));
	wnd.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON_ID));
	wnd.hInstance     = GetModuleHandle(0);
	wnd.lpfnWndProc   = (WNDPROC)WindowProc;
	wnd.lpszClassName = _T("DirectX12");
	wnd.lpszMenuName  = _T("DirectX12");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	RECT rect{};
	rect.bottom = static_cast<long>(height);
	rect.left   = 0;
	rect.right  = static_cast<long>(width);
	rect.top    = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	handle = CreateWindow(wnd.lpszClassName, _T("おかもん"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	if (handle == nullptr)
	{
		OutputDebugString(_T("\nウィンドウの生成：失敗\n"));
		return;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;
	
	ShowWindow(reinterpret_cast<HWND>(handle), SW_SHOW);
}
