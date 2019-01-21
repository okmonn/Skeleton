#include "Window.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "Icon.h"
#include "../etc/Release.h"
#include <Windows.h>
#include <DirectXMath.h>

// リソース数
#define RSC_MAX 1

// コンストラクタ
Window::Window(const Vec2 & size, void * parent) : size(size),
	handle(nullptr), instance(nullptr), name(nullptr), constant(0)
{
	Create(parent);
	ConstantBuffer();
}

// デストラクタ
Window::~Window()
{
	DescriptorMane::Get().DeleteRsc(constant);
	DescriptorMane::Get().DeleteHeap(constant);
	UnregisterClass(name, static_cast<HINSTANCE>(instance));
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
	case WM_DESTROY:
		if (GetParent(reinterpret_cast<HWND>(hWnd)) != nullptr)
		{
			break;
		}
		PostQuitMessage(0);
		return 0;
	case WM_DROPFILES:
		unsigned int fileNum = DragQueryFile(reinterpret_cast<HDROP>(wParam), -1, nullptr, 0);
		if (fileNum > 1)
		{
			break;
		}

		//ファイルパス名のサイズ
		unsigned int size = DragQueryFile(reinterpret_cast<HDROP>(wParam), 0, nullptr, 0);
		HDROP drop = reinterpret_cast<HDROP>(wParam);
		std::wstring name;
		name.resize(size);
		//ファイルパスの取得
		DragQueryFile(drop, 0, &name[0], sizeof(name[0]) * size);

		//if(name.find_last_of())
		break;
	}

	return static_cast<long>(DefWindowProc(reinterpret_cast<HWND>(hWnd), message, wParam, lParam));
}

// ウィンドウの生成
void Window::Create(void * parent)
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
	wnd.lpfnWndProc   = reinterpret_cast<WNDPROC>(WindowProc);
	wnd.lpszClassName = _T("おかもん");
	wnd.lpszMenuName  = _T("おかもん");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	auto flag = (parent == nullptr) ? (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN) : (WS_OVERLAPPEDWINDOW | WS_CHILD);

	RECT rect{};
	rect.bottom = static_cast<long>(size.y);
	rect.left   = 0;
	rect.right  = static_cast<long>(size.x);
	rect.top    = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	size = { static_cast<int>(rect.right), static_cast<int>(rect.bottom) };

	handle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, _T("おかもん"), flag, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), reinterpret_cast<HWND>(parent), nullptr, wnd.hInstance, nullptr);
	if (handle == nullptr)
	{
		OutputDebugString(_T("\nウィンドウの生成：失敗\n"));
		return;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;

	ShowWindow(reinterpret_cast<HWND>(handle), SW_SHOW);
}

// 定数バッファの生成
void Window::ConstantBuffer(void)
{
	DescriptorMane::Get().CreateHeap(constant, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_MAX,
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (sizeof(DirectX::XMFLOAT2) + 0xff) &~0xff;

	DescriptorMane::Get().CreateRsc(constant, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

	DescriptorMane::Get().CBV(constant, constant, (sizeof(DirectX::XMFLOAT2) + 0xff) & ~0xff);

	void* data = nullptr;
	auto hr = DescriptorMane::Get().GetRsc(constant)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nウィンドウ用リソースのマップ：失敗\n"));
		return;
	}

	DirectX::XMFLOAT2 winSize = { static_cast<float>(size.x), static_cast<float>(size.y) };
	memcpy(data, &winSize, sizeof(DirectX::XMFLOAT2));

	DescriptorMane::Get().GetRsc(constant)->Unmap(0, nullptr);
}

// ヒープの取得
ID3D12DescriptorHeap * Window::GetHeap(void)
{
	return DescriptorMane::Get().GetHeap(constant);
}

// リソースの取得
ID3D12Resource * Window::GetRsc(void)
{
	return DescriptorMane::Get().GetRsc(constant);
}
