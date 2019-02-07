#include "Render.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Depth/Depth.h"
#include "../Release.h"
#include <algorithm>

// クリア値
const float color[] = {
	0.0f, 1.0f, 1.0f, 1.0f
};

// コンストラクタ
Render::Render(std::weak_ptr<Swap> swap) : swap(swap), 
	rHeap(nullptr)
{
	rRsc.clear();

	Init();
}

// デストラクタ
Render::~Render()
{
	std::for_each(rRsc.begin(), rRsc.end(), [&](ID3D12Resource* rsc)->void {
		Release(rsc);
	});
	Release(rHeap);
}

// レンダー用ヒープの生成
long Render::CreateRHeap(void)
{
	DXGI_SWAP_CHAIN_DESC1 tmp{};
	swap.lock()->Get()->GetDesc1(&tmp);

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = tmp.BufferCount;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rHeap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nレンダー用ヒープの生成：失敗\n"));
	}

	return hr;
}

// レンダー用リソースの生成
long Render::CreateRRsc(void)
{
	rRsc.resize(rHeap->GetDesc().NumDescriptors);
	auto hr = S_OK;
	for (unsigned int i = 0; i < rRsc.size(); ++i)
	{
		hr = swap.lock()->Get()->GetBuffer(i, IID_PPV_ARGS(&rRsc[i]));
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nレンダー用リソースの生成：失敗\n"));
			return hr;
		}
	}

	return hr;
}

// RTVの生成
void Render::RTV(void)
{
	unsigned int index = 0;
	std::for_each(rRsc.begin(), rRsc.end(), [&](ID3D12Resource* rsc)->void {
		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format        = rsc->GetDesc().Format;
		desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

		auto handle = rHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += Dev->GetDescriptorHandleIncrementSize(rHeap->GetDesc().Type) * index;

		Dev->CreateRenderTargetView(rsc, &desc, handle);
		++index;
	});
}

// 初期化
void Render::Init(void)
{
	CreateRHeap();
	CreateRRsc();
	RTV();
}

// クリア
void Render::Clear(std::weak_ptr<List> list, std::weak_ptr<Depth> depth)
{
	auto handle = rHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(rHeap->GetDesc().Type) * swap.lock()->Get()->GetCurrentBackBufferIndex();

	auto dHandle = depth.lock()->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	list.lock()->GetList()->OMSetRenderTargets(1, &handle, false, &dHandle);

	list.lock()->GetList()->ClearRenderTargetView(handle, color, 0, nullptr);
}

// リソースの取得
ID3D12Resource * Render::GetRsc(void) const
{
	return rRsc[swap.lock()->Get()->GetCurrentBackBufferIndex()];
}
