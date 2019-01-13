#include "Depth.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 1

// コンストラクタ
Depth::Depth(std::weak_ptr<Window>win) : win(win),
	heap(0), rsc(0)
{
	Create();
}

// デストラクタ
Depth::~Depth()
{
	DescriptorMane::Get().DeleteRsc(rsc);
	DescriptorMane::Get().DeleteHeap(heap);
}

// デプスの生成
void Depth::Create(void)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE, RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.Height           = static_cast<unsigned int>(win.lock()->GetSize().y);
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels        = 0;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = static_cast<unsigned __int64>(win.lock()->GetSize().x);

	D3D12_CLEAR_VALUE clear{};
	clear.DepthStencil = { 1.0f, 0 };
	clear.Format       = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	DescriptorMane::Get().CreateRsc(rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear);

	DescriptorMane::Get().DSV(heap, rsc);
}

// クリア
void Depth::Clear(std::weak_ptr<List> list)
{
	auto handle = DescriptorMane::Get().GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	list.lock()->GetList()->ClearDepthStencilView(handle, D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

// ヒープの取得
ID3D12DescriptorHeap * Depth::GetHeap(void)
{
	return DescriptorMane::Get().GetHeap(heap);
}

// リソースの取得
ID3D12Resource * Depth::GetRsc(void)
{
	return DescriptorMane::Get().GetRsc(rsc);
}
