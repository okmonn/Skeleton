#include "MultiPass.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Queue/Queue.h"
#include "../Depth/Depth.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"
#include <DirectXMath.h>

const float color[] = {
	1.0f, 1.0f, 1.0f, 1.0f
};

struct Vertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

// コンストラクタ
MultiPass::MultiPass(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list) :
	descMane(DescriptorMane::Get()), win(win), dev(dev), list(list), rHeap(0), sHeap(0), rsc(0), vRsc(0)
{
	Init();
}

// デストラクタ
MultiPass::~MultiPass()
{
	descMane.DeleteRsc(vRsc);
	descMane.DeleteRsc(rsc);
	descMane.DeleteHeap(sHeap);
	descMane.DeleteHeap(rHeap);
}

// リソースの生成
long MultiPass::CreateRsc(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width            = win.lock()->GetX();
	desc.Height           = win.lock()->GetY();
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE clear{};
	for (int i = 0; i < _countof(color); ++i)
	{
		clear.Color[i] = color[i];
	}
	clear.Format = desc.Format;

	return descMane.CreateRsc(dev, rsc, prop, desc, &clear, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

// レンダーターゲットビューの生成
void MultiPass::CreateRenderView(void)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	dev.lock()->Get()->CreateRenderTargetView(descMane.GetRsc(rsc), &desc, descMane.GetHeap(rHeap)->GetCPUDescriptorHandleForHeapStart());
}

// シェーダービューの生成
void MultiPass::CreateShaderView(void)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(descMane.GetRsc(rsc), &desc, descMane.GetHeap(sHeap)->GetCPUDescriptorHandleForHeapStart());
}

// 頂点リソースの生成	
long MultiPass::CreateVertexRsc(void)
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
	desc.Width            = sizeof(Vertex) * 4;

	return descMane.CreateRsc(dev, vRsc, prop, desc);
}

// 頂点マップ
long MultiPass::Map(void)
{
	Vertex vertex[] = {
		{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
	};

	void* data = nullptr;
	auto hr = descMane.GetRsc(vRsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		return hr;
	}

	memcpy(data, &vertex[0], sizeof(vertex));

	descMane.GetRsc(vRsc)->Unmap(0, nullptr);

	return hr;
}

// 初期化
void MultiPass::Init(void)
{
	descMane.CreateHeap(dev, rHeap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descMane.CreateHeap(dev, sHeap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CreateRsc();

	CreateRenderView();
	CreateShaderView();

	CreateVertexRsc();
	Map();
}

// レンダーターゲットのクリア
void MultiPass::Clear(std::weak_ptr<Depth> depth)
{
	list.lock()->GetList()->OMSetRenderTargets(1, &descMane.GetHeap(rHeap)->GetCPUDescriptorHandleForHeapStart(), false, &depth.lock()->GetHeap()->GetCPUDescriptorHandleForHeapStart());
	list.lock()->GetList()->ClearRenderTargetView(descMane.GetHeap(rHeap)->GetCPUDescriptorHandleForHeapStart(), color, 0, nullptr);
}

// 描画準備
void MultiPass::Set(std::weak_ptr<Depth> depth)
{
	list.lock()->Reset(nullptr);

	list.lock()->SetView(win.lock()->GetX(), win.lock()->GetY());

	list.lock()->SetScissor(win.lock()->GetX(), win.lock()->GetY());

	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		descMane.GetRsc(rsc));

	depth.lock()->Clear(list);

	Clear(depth);
}

// 実行
void MultiPass::Execution(std::weak_ptr<Queue> queue, std::weak_ptr<Fence> fence)
{
	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		descMane.GetRsc(rsc));

	list.lock()->GetList()->Close();

	//コマンド実行
	ID3D12CommandList* ppCmdLists[] = {
		list.lock()->GetList(),
	};
	queue.lock()->Get()->ExecuteCommandLists(_countof(ppCmdLists), ppCmdLists);

	fence.lock()->Wait();
}

// 描画
void MultiPass::Draw(std::weak_ptr<List> list, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe)
{
	list.lock()->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(Vertex) * 4;
	view.StrideInBytes  = sizeof(Vertex);
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	auto heap = descMane.GetHeap(sHeap);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	list.lock()->GetList()->DrawInstanced(4, 1, 0, 0);
}
