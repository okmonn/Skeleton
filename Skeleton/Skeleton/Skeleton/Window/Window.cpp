#include "Window.h"
#include "Icon.h"
#include "../Useful/Useful.h"
#include "../Release.h"
#include <Windows.h>

std::tstring Window::dropFilePass;

// リソース数
#define RSC_NUM 1

// コンストラクタ
Window::Window(const Vec2 & size, void * parent) : size(size), 
	handle(nullptr), instance(nullptr), name(nullptr), cHeap(nullptr), cRsc(nullptr)
{
	CreateWnd(parent);
	Init();
}

// デストラクタ
Window::~Window()
{
	Release(cRsc);
	Release(cHeap);
	UnregisterClass(name, static_cast<HINSTANCE>(instance));
	DestroyWindow(reinterpret_cast<HWND>(handle));
}

// ウィンドウのコールバック
#if _WIN64
long __stdcall Window::WindowProc(void * hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
#else
long __stdcall Window::WindowProc(void* hWnd, unsigned int message, long wParam, long lParam)
#endif
{
	unsigned int fileNum = 0;
	unsigned int size = 0;
	HDROP drop = nullptr;

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
		fileNum = DragQueryFile(reinterpret_cast<HDROP>(wParam), -1, nullptr, 0);
		if (fileNum > 1)
		{
			break;
		}

		//ファイルパス名のサイズ
		size = DragQueryFile(reinterpret_cast<HDROP>(wParam), 0, nullptr, 0);
		drop = reinterpret_cast<HDROP>(wParam);
		dropFilePass.resize(size);
		//ファイルパスの取得
		DragQueryFile(drop, 0, &dropFilePass[0], sizeof(dropFilePass[0]) * size);
		break;
	default:
		break;
	}

	return static_cast<long>(DefWindowProc(reinterpret_cast<HWND>(hWnd), message, wParam, lParam));
}

// ウィンドウの生成
int Window::CreateWnd(void * parent)
{
	WNDCLASSEX wnd{};
	wnd.cbClsExtra    = 0;
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.cbWndExtra    = 0;
	wnd.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
	wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon         = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON_ID));
	wnd.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON_ID));
	wnd.hInstance     = GetModuleHandle(0);
	wnd.lpfnWndProc   = reinterpret_cast<WNDPROC>(WindowProc);
	wnd.lpszClassName = _T("おかもん");
	wnd.lpszMenuName  = _T("おかもん");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	auto flag = (parent == nullptr) ? (WS_OVERLAPPEDWINDOW) : (WS_OVERLAPPEDWINDOW | WS_POPUP);

	RECT rect{};
	rect.bottom = static_cast<long>(size.y);
	rect.left   = 0;
	rect.right  = static_cast<long>(size.x);
	rect.top    = 0;
	AdjustWindowRect(&rect, flag, false);

	auto disSize = use::GetDisplayResolution();
	Vec2 pos = { use::GetRand(0, disSize.x - size.x), use::GetRand(0, disSize.y - size.y) };

	handle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, _T("おかもん"), flag, pos.x, pos.y,
		(rect.right - rect.left), (rect.bottom - rect.top), reinterpret_cast<HWND>(parent), nullptr, wnd.hInstance, nullptr);
	if (handle == nullptr)
	{
		OutputDebugString(_T("\nウィンドウの生成：失敗\n"));
		return -1;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;

	ShowWindow(reinterpret_cast<HWND>(handle), SW_SHOW);

	return 0;
}

// 定数用ヒープの生成
long Window::CreateCHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = RSC_NUM;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&cHeap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nウィンドウ用定数ヒープの生成：失敗\n"));
	}

	return hr;
}

// 定数リソースの生成
long Window::CreateCRsc(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (sizeof(DirectX::XMFLOAT2) + 0xff) &~ 0xff;

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cRsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nウィンドウ用定数リソースの生成：失敗\n"));
	}

	return hr;
}

// CBVの生成
void Window::CBV(void)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = cRsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = static_cast<unsigned int>(cRsc->GetDesc().Width);

	auto handle = cHeap->GetCPUDescriptorHandleForHeapStart();
	Dev->CreateConstantBufferView(&desc, handle);
}

// マップ
long Window::Map(void)
{
	void* data = nullptr;
	auto hr = cRsc->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nウィンドウ用リソースのマップ：失敗\n"));
		return hr;
	}

	DirectX::XMFLOAT2 tmp = { static_cast<float>(size.x), static_cast<float>(size.y) };
	memcpy(data, &tmp, sizeof(DirectX::XMFLOAT2));

	UnMap(cRsc);

	return hr;
}

// 初期化
void Window::Init(void)
{
	CreateCHeap();
	CreateCRsc();
	CBV();
	Map();
}

// ドロップされたファイルパスの取得
std::string Window::GetDropPass(void)
{
	auto tmp = dropFilePass;
	dropFilePass.clear();

#ifdef UNICODE
	return use::ChangeCode(tmp);
#else
	return tmp;
#endif
}
