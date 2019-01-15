#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/RootMane.h"
#include "../Pipe/PipeMane.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// コンストラクタ
Compute::Compute() : 
	heap(0)
{
	info.clear();

	queue = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(queue);
}

// デストラクタ
Compute::~Compute()
{
}

// シェーダの読み込み
void Compute::Load(const std::string & name, const std::wstring & fileName)
{
	RootMane::Get().Create(name, fileName);
	PipeMane::Get().Create(name, RootMane::Get().GetRoot(name));
}

// CBVの生成
void Compute::CBV(const std::string & name, const size_t & size, const unsigned int & index)
{
	if (info.find(name) != info.end())
	{
		return;
	}

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
	desc.Width            = static_cast<unsigned __int64>(size);

	DescriptorMane::Get().CreateRsc(info[name].rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

	info[name].rsc = heap++;

	DescriptorMane::Get().CBV(heap, info[name].rsc, size, info[name].rsc);
}

// UAVの生成
void Compute::UAV(const std::string & name, const size_t & stride, const size_t & num, const unsigned int & index)
{
	if (info.find(name) != info.end())
	{
		return;
	}

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = static_cast<unsigned __int64>(stride * num);

	DescriptorMane::Get().CreateRsc(info[name].rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	info[name].rsc = heap++;

	DescriptorMane::Get().UAV(heap, info[name].rsc, stride, num, info[name].rsc);
}

// マップ
long Compute::Map(const std::string & name)
{
	if (info.find(name) != info.end())
	{
		return S_FALSE;
	}

	D3D12_RANGE range{ 0, 1 };
	auto hr = DescriptorMane::Get().GetRsc(info[name].rsc)->Map(0, &range, &info[name].data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用リソースのマップ：失敗\n"));
	}

	return hr;
}

// アンマップ
void Compute::Unmap(const std::string & name)
{
	if (info.find(name) != info.end())
	{
		return;
	}

	D3D12_RANGE range{ 0, 1 };
	DescriptorMane::Get().GetRsc(info[name].rsc)->Unmap(0, &range);
}

// 終了
void Compute::End(void)
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Unmap(itr->first);
		DescriptorMane::Get().DeleteRsc(itr->second.rsc);
	}
	DescriptorMane::Get().DeleteHeap(heap);
}
