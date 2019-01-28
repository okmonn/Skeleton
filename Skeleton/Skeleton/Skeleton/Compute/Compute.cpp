#include "Compute.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Release.h"

// コンストラクタ
Compute::Compute() : 
	heap(nullptr), index(0)
{
	rsc.clear();
	data.clear();

	queue = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(queue);
}

// デストラクタ
Compute::~Compute()
{
}

// ルート・パイプの生成
void Compute::Load(const std::string & name, const std::string & fileName)
{
	RootMane::Get().Create(name, fileName);
	PipeMane::Get().Create(name, RootMane::Get().GetRoot(name));
}

// ヒープの生成
long Compute::CreateHeap(const unsigned int & num)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = num;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用ヒープの生成：失敗\n"));
	}

	return hr;
}

// マップ
long Compute::Map(const std::string & name)
{
	if (rsc.find(name) == rsc.end())
	{
		return S_FALSE;
	}

	D3D12_RANGE range{ 0, 1 };
	auto hr = rsc[name]->Map(0, &range, &data[name]);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用リソースのマップ：失敗\n"));
	}

	return hr;
}

// 定数リソースの生成
long Compute::CreateCRsc(const std::string & name, const size_t & size)
{
	if (rsc.find(name) != rsc.end())
	{
		OutputDebugString(_T("\nコンピュート用定数リソース生成済み\n"));
		return S_FALSE;
	}

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
	desc.Width            = (size + 0xff) &~0xff;

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc[name]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用定数リソースの生成：失敗\n"));
	}

	return hr;
}

// CBVの生成
void Compute::CBV(const std::string & name, const size_t & size)
{
	if (FAILED(CreateCRsc(name, size)))
	{
		return;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = rsc[name]->GetGPUVirtualAddress();
	desc.SizeInBytes    = static_cast<unsigned int>(rsc[name]->GetDesc().Width);

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index++;
	Dev->CreateConstantBufferView(&desc, handle);

	Map(name);
}

// 非順序リソースの生成
long Compute::CreateURsc(const std::string & name, const size_t & stride, const size_t & num)
{
	if (rsc.find(name) != rsc.end())
	{
		OutputDebugString(_T("\nコンピュート用非順序リソース生成済み\n"));
		return S_FALSE;
	}

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;

	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = static_cast<unsigned __int64>(stride * num);

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&rsc[name]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用非順序リソースの生成：失敗\n"));
	}

	return hr;
}

// UAVの生成
void Compute::UAV(const std::string & name, const size_t & stride, const size_t & num)
{
	if (FAILED(CreateURsc(name, stride, num)))
	{
		return;
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = static_cast<unsigned int>(num);
	desc.Buffer.StructureByteStride = static_cast<unsigned int>(stride);

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index++;
	Dev->CreateUnorderedAccessView(rsc[name], nullptr, &desc, handle);

	Map(name);
}

// アンマップ
void Compute::Unmap(const std::string & name)
{
	if (rsc.find(name) == rsc.end())
	{
		return;
	}

	D3D12_RANGE range{ 0, 1 };
	rsc[name]->Unmap(0, &range);
}

// データのリセット
void Compute::Reset(const std::string & name)
{
	if (data.find(name) == data.end())
	{
		return;
	}

	memset(data[name], 0, static_cast<size_t>(rsc[name]->GetDesc().Width));
}

// 終了
void Compute::End(void)
{
	for (auto itr = rsc.begin(); itr != rsc.end(); ++itr)
	{
		Unmap(itr->first);
		Release(itr->second);
	}
	Release(heap);
}
