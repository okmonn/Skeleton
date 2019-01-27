#include "Depth.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Release.h"

// リソース数
#define RSC_NUM 1

// コンストラクタ
Depth::Depth(std::weak_ptr<Window> win) : win(win),
	dHeap(nullptr), dRsc(nullptr)
{
	Init();
}

// デストラクタ
Depth::~Depth()
{
	Release(dRsc);
	Release(dHeap);
}

// デプス用ヒープの生成
long Depth::CreateDHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = RSC_NUM;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	auto hr = Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&dHeap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nデプス用ヒープの生成：失敗\n"));
	}

	return hr;
}

// デプス用リソースの生成
long Depth::CreateDRsc(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.Height           = static_cast<unsigned int>(win.lock()->GetSize().y);
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = static_cast<unsigned __int64>(win.lock()->GetSize().x);

	D3D12_CLEAR_VALUE clear{};
	clear.DepthStencil = { 1.0f, 0 };
	clear.Format       = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&dRsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nデプス用リソースの生成：失敗\n"));
	}

	return hr;
}

// DSVの生成
void Depth::DSV(void)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Flags         = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	desc.Format        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	auto handle = dHeap->GetCPUDescriptorHandleForHeapStart();
	Dev->CreateDepthStencilView(dRsc, &desc, handle);
}

// 初期化
void Depth::Init(void)
{
	CreateDHeap();
	CreateDRsc();
	DSV();
}

// クリア
void Depth::Clear(std::weak_ptr<List> list)
{
	auto handle = dHeap->GetCPUDescriptorHandleForHeapStart();
	list.lock()->GetList()->ClearDepthStencilView(handle, D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
