#include "Primitive.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Release.h"

// コンストラクタ
Primitive::Primitive(std::weak_ptr<Window> win, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe, const size_t & num) : win(win), root(root), pipe(pipe),
	rsc(nullptr)
{
	CreateVRsc(num);
}

// デストラクタ
Primitive::~Primitive()
{
	Release(rsc);
}

// 頂点リソースの生成
long Primitive::CreateVRsc(const size_t & num)
{
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
	desc.Width            = sizeof(prm::Vertex) * num;

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用頂点リソースの生成：失敗\n"));
	}

	return hr;
}

// マップ
long Primitive::Map(const std::vector<prm::Vertex>& vertex)
{
	void* data = nullptr;
	auto hr = rsc->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(prm::Vertex) * vertex.size());
	UnMap(rsc);

	return hr;
}

// 描画
void Primitive::Draw(std::weak_ptr<List> list, const std::vector<prm::Vertex>& vertex, const D3D_PRIMITIVE_TOPOLOGY & type)
{
	Map(vertex);

	list.lock()->Root(root.lock()->Get());
	list.lock()->Pipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(rsc->GetDesc().Width);
	view.StrideInBytes  = sizeof(prm::Vertex);
	list.lock()->VertexView(view);

	auto heap = win.lock()->GetHeap();
	list.lock()->Heap(&heap, 1);
	list.lock()->RootTable(0, heap);

	list.lock()->Topology(type);

	list.lock()->DrawVertex(vertex.size());
}
