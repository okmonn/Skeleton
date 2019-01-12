#include "Primitive.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 1

// コンストラクタ
Primitive::Primitive() : 
	constant(0), vRsc(0)
{
	vertex.clear();

	list = std::make_unique<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
}

// デストラクタ
Primitive::~Primitive()
{
}

// 定数バッファの生成
void Primitive::ConstantBuffer(void)
{
	DescriptorMane::Get().CreateHeap(constant, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_MAX,
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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
	desc.Width            = (sizeof(DirectX::XMFLOAT2) + 0xff) &~ 0xff;

	DescriptorMane::Get().CreateRsc(constant, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

	DescriptorMane::Get().CBV(constant, constant, (sizeof(DirectX::XMFLOAT2) + 0xff) & ~0xff);

	void* data = nullptr;
	auto hr = DescriptorMane::Get().GetRsc(constant)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用定数リソースのマップ：失敗\n"));
		return;
	}

	DirectX::XMFLOAT2 winSize = { static_cast<float>(win.lock()->GetWidth()), static_cast<float>(win.lock()->GetHeight()) };
	memcpy(data, &winSize, sizeof(DirectX::XMFLOAT2));

	DescriptorMane::Get().GetRsc(constant)->Unmap(0, nullptr);
}

// 頂点バッファの生成
void Primitive::VertexBuffer(void)
{
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
	desc.Width            = sizeof(prm::Vertex) * vertex.size();

	DescriptorMane::Get().CreateRsc(vRsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

	void* data = nullptr;
	auto hr = DescriptorMane::Get().GetRsc(vRsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用頂点リソースのマップ：失敗\n"));
		return;
	}

	memcpy(data, vertex.data(), sizeof(prm::Vertex) * vertex.size());

	DescriptorMane::Get().GetRsc(vRsc)->Unmap(0, nullptr);
}

// バンドルのセット
void Primitive::Bundle(const D3D_PRIMITIVE_TOPOLOGY & type)
{
	list->Reset(pipe.lock()->Get());

	list->SetRoot(root.lock()->Get());
	list->SetPipe(pipe.lock()->Get());

	auto heap = DescriptorMane::Get().GetHeap(constant);
	list->SetHeap(&heap, 1);
	list->SetRootTable(0, heap);

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = DescriptorMane::Get().GetRsc(vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(sizeof(prm::Vertex) * vertex.size());
	view.StrideInBytes  = sizeof(prm::Vertex);
	list->SetVertexBufferView(view);

	list->SetTopology(type);

	list->DrawVertex(vertex.size());

	list->Close();
}

// 描画
void Primitive::Draw(std::weak_ptr<List> list)
{
	auto heap = DescriptorMane::Get().GetHeap(constant);
	list.lock()->SetHeap(&heap, 1);
	list.lock()->ExecuteBundle(this->list->GetList());
}
