#include "Render.h"
#include "../Device/Device.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// クリア値
const float color[] = {
	1.0f, 1.0f, 1.0f, 1.0f
};

// コンストラクタ
Render::Render(std::weak_ptr<Swap>swap) : swap(swap), 
	heap(0)
{
	rsc.clear();

	Create();
}

// デストラクタ
Render::~Render()
{
	for (auto& i : rsc)
	{
		DescriptorMane::Get().DeleteRsc(i);
	}
	DescriptorMane::Get().DeleteHeap(heap);
}

// レンダーの生成
void Render::Create(void)
{
	DXGI_SWAP_CHAIN_DESC1 swapDesc{};
	swap.lock()->Get()->GetDesc1(&swapDesc);
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE, swapDesc.BufferCount, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	rsc.resize(swapDesc.BufferCount);
	for (size_t i = 0; i < rsc.size(); ++i)
	{
		DescriptorMane::Get().CreateRsc(rsc[i], swap, i);
		DescriptorMane::Get().RTV(heap, rsc[i], i);
	}
}

// クリア
void Render::Clear(const std::weak_ptr<List>list, ID3D12DescriptorHeap * depth)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = DescriptorMane::Get().GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * swap.lock()->Get()->GetCurrentBackBufferIndex();

	auto heapHandle = depth == nullptr ? nullptr : &depth->GetCPUDescriptorHandleForHeapStart();
	list.lock()->GetList()->OMSetRenderTargets(1, &handle, false, heapHandle);

	list.lock()->GetList()->ClearRenderTargetView(handle, color, 0, nullptr);
}

// ヒープの取得
ID3D12DescriptorHeap * Render::GetHeap(void)
{
	return DescriptorMane::Get().GetHeap(heap);
}

// リソースの取得
ID3D12Resource * Render::GetRsc(void)
{
	return DescriptorMane::Get().GetRsc(rsc[swap.lock()->Get()->GetCurrentBackBufferIndex()]);
}

// リソースの取得
ID3D12Resource * Render::GetRsc(const unsigned int & i)
{
	return DescriptorMane::Get().GetRsc(rsc[i]);
}
