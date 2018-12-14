#include <d3dcompiler.h>
#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// デストラクタ
Compute::Compute() : 
	descMane(DescriptorMane::Get()), root(nullptr), pipe(nullptr), heap(0)
{
	info.clear();
}

// デストラクタ
Compute::~Compute()
{
}

// クラスのインスタンス
void Compute::Create(void)
{
	queue = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, queue);
}

// ルートシグネチャの生成
long Compute::CreateRoot(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートシェーダの読み込み：失敗\n"));
		return hr;
	}

	ID3DBlob* sig = nullptr;

	hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャ情報の取得：失敗\n"));
		return hr;
	}

	hr = dev.lock()->Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャの生成：失敗\n"));
	}

	Release(sig);

	return hr;
}

// パイプラインの生成
long Compute::CreatePipe(void)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength  = shader->GetBufferSize();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root;

	auto hr = dev.lock()->Get()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートパイプラインの生成：失敗\n"));
	}

	return hr;
}

// UAVリソースの生成
long Compute::CreateUavRsc(const std::string & name, const unsigned int & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 1;

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
	desc.Width            = (size + 0xff) &~0xff;

	info[name].rsc = heap++;

	return descMane.CreateRsc(dev, info[name].rsc, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

// UAVの生成
void Compute::CreateUnorderView(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = num;
	desc.Buffer.StructureByteStride = stride;

	auto handle = descMane.GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].rsc * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dev.lock()->Get()->CreateUnorderedAccessView(descMane.GetRsc(info[name].rsc), nullptr, &desc, handle);
}

// CBVリソースの生成
long Compute::CreateCbvRsc(const std::string & name, const unsigned int & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

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
	desc.Width            = (size + 0xff) &~0xff;

	info[name].rsc = heap++;

	return descMane.CreateRsc(dev, info[name].rsc, prop, desc);
}

// CBVの生成
void Compute::CreateConstantView(const std::string & name, const unsigned int & size)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(info[name].rsc)->GetGPUVirtualAddress();
	desc.SizeInBytes    = (size + 0xff) &~0xff;

	auto handle = descMane.GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].rsc * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// マップ
long Compute::Map(const std::string & name)
{
	D3D12_RANGE range{ 0, 1 };

	auto hr = descMane.GetRsc(info[name].rsc)->Map(0, &range, &info[name].data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートリソースのマップ：失敗\n"));
	}

	return hr;
}

// アンマップ
void Compute::UnMap(const std::string & name)
{
	D3D12_RANGE range{ 0, 1 };

	descMane.GetRsc(info[name].rsc)->Unmap(0, &range);
}

// UAVの生成
void Compute::UAV(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	CreateUavRsc(name, stride * num);
	CreateUnorderView(name, stride, num);
	Map(name);
}

// CBVの生成
void Compute::CBV(const std::string & name, const unsigned int & size)
{
	CreateCbvRsc(name, size);
	CreateConstantView(name, size);
	Map(name);
}

// 終了
void Compute::End(void)
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		UnMap(itr->first);
		descMane.DeleteRsc(itr->second.rsc);
	}
	
	descMane.DeleteHeap(heap);
}
