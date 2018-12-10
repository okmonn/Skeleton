#include "ShadowMap.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Queue/Queue.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// コンストラクタ
ShadowMap::ShadowMap(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list) :
	descMane(DescriptorMane::Get()), win(win), dev(dev), queue(queue), list(list), dHeap(0), sHeap(0), rsc(0)
{
	queue = std::make_shared<Queue>(dev);
	fence = std::make_unique<Fence>(dev, queue);

	Init();
}

// デストラクタ
ShadowMap::~ShadowMap()
{
	descMane.DeleteRsc(rsc);
	descMane.DeleteHeap(sHeap);
	descMane.DeleteHeap(dHeap);
}

// リソースの生成
long ShadowMap::CreateRsc(void)
{
	auto size = max(win.lock()->GetX(), win.lock()->GetY());
	size = (unsigned int)ceilf(log2f(static_cast<float>(size)));
	size = (unsigned int)powf(2.0f, static_cast<float>(size));

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
	desc.Height           = size;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels        = 0;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (UINT64)size;

	D3D12_CLEAR_VALUE clear{};
	clear.DepthStencil.Depth   = 1.0f;
	clear.DepthStencil.Stencil = 0;
	clear.Format               = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	return descMane.CreateRsc(dev, rsc, prop, desc, &clear, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

// 深度ビューの生成
void ShadowMap::CreateDepthView(void)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Flags              = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	dev.lock()->Get()->CreateDepthStencilView(descMane.GetRsc(rsc), &desc, descMane.GetHeap(dHeap)->GetCPUDescriptorHandleForHeapStart());
}

// シェーダービューの生成
void ShadowMap::CreateShaderView(void)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(descMane.GetRsc(rsc), &desc, descMane.GetHeap(sHeap)->GetCPUDescriptorHandleForHeapStart());
}

// 初期化
void ShadowMap::Init(void)
{
	descMane.CreateHeap(dev, dHeap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	descMane.CreateHeap(dev, sHeap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	CreateRsc();
	CreateDepthView();
	CreateShaderView();
}

// 描画準備
void ShadowMap::Set(void)
{
	list.lock()->Reset();

	list.lock()->SetView((unsigned int)descMane.GetRsc(rsc)->GetDesc().Width, descMane.GetRsc(rsc)->GetDesc().Height);
	list.lock()->SetScissor((unsigned int)descMane.GetRsc(rsc)->GetDesc().Width, descMane.GetRsc(rsc)->GetDesc().Height);

	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		descMane.GetRsc(rsc));

	list.lock()->GetList()->OMSetRenderTargets(0, nullptr, false, &descMane.GetHeap(dHeap)->GetCPUDescriptorHandleForHeapStart());
	list.lock()->GetList()->ClearDepthStencilView(descMane.GetHeap(dHeap)->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

// 描画実行
void ShadowMap::Execution(void)
{
	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		descMane.GetRsc(rsc));

	list.lock()->GetList()->Close();

	//コマンド実行
	ID3D12CommandList* ppCmdLists[] = {
		list.lock()->GetList(),
	};
	queue->Get()->ExecuteCommandLists(_countof(ppCmdLists), ppCmdLists);

	fence->Wait();
}
