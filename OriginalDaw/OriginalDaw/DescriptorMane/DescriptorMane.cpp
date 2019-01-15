#include "DescriptorMane.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// コンストラクタ
DescriptorMane::DescriptorMane()
{
	heap.clear();
	rsc.clear();
}

// デストラクタ
DescriptorMane::~DescriptorMane()
{
	for (auto& i : heap)
	{
		Release(i.second);
	}
	for (auto& i : rsc)
	{
		Release(i.second);
	}
}

// ヒープの生成
long DescriptorMane::CreateHeap(int& addr, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag, const size_t & num, const D3D12_DESCRIPTOR_HEAP_TYPE & type)
{
	if (heap.find(&addr) != heap.end())
	{
		OutputDebugString(_T("\n生成済み\n"));
		return S_FALSE;
	}

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = static_cast<unsigned int>(num);
	desc.Type           = type;

	auto hr = Device::Get().GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap[&addr]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long DescriptorMane::CreateRsc(int & addr, const D3D12_HEAP_PROPERTIES & prop, const D3D12_RESOURCE_DESC & desc, const D3D12_RESOURCE_STATES & state, const D3D12_CLEAR_VALUE * clear)
{
	if (rsc.find(&addr) != rsc.end())
	{
		OutputDebugString(_T("\n生成済み\n"));
		return S_FALSE;
	}

	auto hr = Device::Get().GetDev()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, state, clear, IID_PPV_ARGS(&rsc[&addr]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリソースの生成：失敗\n"));
		addr = -1;
		return hr;
	}

	addr = RSC_TYPE::RSC_TYPE_NON;
	return hr;
}

// リソースの生成
long DescriptorMane::CreateRsc(int & addr, std::weak_ptr<Swap> swap, const unsigned int & index)
{
	auto hr = swap.lock()->Get()->GetBuffer(index, IID_PPV_ARGS(&rsc[&addr]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリソースの生成：失敗\n"));
		addr = -1;
		return hr;
	}

	addr = RSC_TYPE::RSC_TYPE_NON;
	return hr;
}

// RTVの生成
void DescriptorMane::RTV(int & heapAddr, int & rscAddr, const unsigned int & index)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format               = rsc[&rscAddr]->GetDesc().Format;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	auto handle = heap[&heapAddr]->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * index;

	Device::Get().GetDev()->CreateRenderTargetView(rsc[&rscAddr], &desc, handle);
	rscAddr = RSC_TYPE::RSC_TYPE_RTV;
}

// DSVの生成
void DescriptorMane::DSV(int & heapAddr, int & rscAddr, const unsigned int & index)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Flags              = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	auto handle = heap[&heapAddr]->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV) * index;

	Device::Get().GetDev()->CreateDepthStencilView(rsc[&rscAddr], &desc, handle);
	rscAddr = RSC_TYPE::RSC_TYPE_DSV;
}

// CBVの生成
void DescriptorMane::CBV(int & heapAddr, int & rscAddr, const size_t & size, const unsigned int & index)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = rsc[&rscAddr]->GetGPUVirtualAddress();
	desc.SizeInBytes    = (size + 0xff) &~ 0xff;

	auto handle = heap[&heapAddr]->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

	Device::Get().GetDev()->CreateConstantBufferView(&desc, handle);
	rscAddr = RSC_TYPE::RSC_TYPE_CBV;
}

// SRVの生成
void DescriptorMane::SRV(int & heapAddr, int & rscAddr, const unsigned int & index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = rsc[&rscAddr]->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto handle = heap[&heapAddr]->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

	Device::Get().GetDev()->CreateShaderResourceView(rsc[&rscAddr], &desc, handle);
	rscAddr = RSC_TYPE::RSC_TYPE_SRV;
}

// UAVの生成
void DescriptorMane::UAV(int & heapAddr, int & rscAddr, const size_t & stride, const size_t & num, const unsigned int & index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = static_cast<unsigned int>(num);
	desc.Buffer.StructureByteStride = static_cast<unsigned int>(stride);

	auto handle = heap[&heapAddr]->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

	Device::Get().GetDev()->CreateUnorderedAccessView(rsc[&rscAddr], nullptr, &desc, handle);
}

// ヒープの削除
void DescriptorMane::DeleteHeap(int & addr)
{
	if (heap.find(&addr) != heap.end())
	{
		Release(heap[&addr]);
		heap.erase(heap.find(&addr));
	}
}

// リソースの削除
void DescriptorMane::DeleteRsc(int & addr)
{
	if (rsc.find(&addr) != rsc.end())
	{
		Release(rsc[&addr]);
		rsc.erase(rsc.find(&addr));
	}
}
