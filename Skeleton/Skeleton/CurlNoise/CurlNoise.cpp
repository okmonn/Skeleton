#include <d3dcompiler.h>
#include "CurlNoise.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 1

// コンストラクタ
CurlNoise::CurlNoise(std::weak_ptr<Device>dev, const std::tstring & fileName) :
	dev(dev), shader(nullptr), root(nullptr), pipe(nullptr), heap(nullptr), index(0)
{
	info.clear();

	queue = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, queue);

	CreateRoot(fileName);
	CreatePipe();
	CreateHeap();
}

// デストラクタ
CurlNoise::~CurlNoise()
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Release(itr->second.rsc);
	}
	Release(heap);
	Release(pipe);
	Release(root);
	Release(shader);
}

// ルートシグネチャの生成
long CurlNoise::CreateRoot(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
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
long CurlNoise::CreatePipe(void)
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

// ヒープの生成
long CurlNoise::CreateHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nカールノイズ用ヒープの生成：失敗\n"));
	}

	return hr;
}

// UAVリソースの生成
long CurlNoise::CreateUavRsc(const std::string & name, const unsigned int & size)
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

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&info[name].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nカールノイズ用UAVリソースの生成：失敗\n"));
		return hr;
	}

	info[name].index = index++;

	return hr;
}

// UAVの生成
void CurlNoise::CreateUnorderView(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = num;
	desc.Buffer.StructureByteStride = stride;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].index * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dev.lock()->Get()->CreateUnorderedAccessView(info[name].rsc, nullptr, &desc, handle);
}

// マップ
long CurlNoise::Map(const std::string & name)
{
	D3D12_RANGE range{ 0, 1 };

	auto hr = info[name].rsc->Map(0, &range, reinterpret_cast<void**>(&info[name].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nカールノイズ用リソースのマップ：失敗\n"));
	}

	return hr;
}

// UAVの生成
void CurlNoise::UAV(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	CreateUavRsc(name, stride * num);
	CreateUnorderView(name, stride, num);
	Map(name);
}

// データの初期化
void CurlNoise::Init(const std::vector<DirectX::XMFLOAT3>& pos)
{
	this->pos = pos;
}

// 実行
void CurlNoise::Execution(void)
{
	list->Reset();

	list->GetList()->SetComputeRootSignature(root);
	list->GetList()->SetPipelineState(pipe);

	list->GetList()->SetDescriptorHeaps(1, &heap);

	auto size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u0"].index;
	list->GetList()->SetComputeRootDescriptorTable(0, handle);

	list->GetList()->Dispatch(static_cast<unsigned int>(pos.size()), 1, 1);

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};

	queue->Get()->ExecuteCommandLists(1, com);

	fence->Wait();
}
