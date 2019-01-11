#include "Swap.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// コンストラクタ
Swap::Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue) : win(win), queue(queue), 
	swap(nullptr)
{
	Create();
}

// デストラクタ
Swap::~Swap()
{
	Release(swap);
}

// スワップチェインの生成
long Swap::Create(void)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = 0;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = win.lock()->GetHeight();
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = win.lock()->GetWidth();

	IDXGIFactory4* fac = nullptr;
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&fac));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nファクトリーの生成：失敗\n"));
		return hr;
	}

	hr = fac->CreateSwapChainForHwnd(queue.lock()->Get(), reinterpret_cast<HWND>(win.lock()->Get()), &desc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nスワップチェインの生成：失敗\n"));
	}

	Release(fac);

	return hr;
}

// 裏・表画面の遷移
void Swap::Present(void)
{
	swap->Present(1, 0);
}
