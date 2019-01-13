#include "Primitive.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// トポロジー
const D3D12_PRIMITIVE_TOPOLOGY topology[] = {
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};

// コンストラクタ
Primitive::Primitive(std::weak_ptr<Window> win, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe, const prm::PrimitiveType & type) : 
	win(win), root(root), pipe(pipe), type(type), rsc(0)
{
	vertex.clear();

	VertexBuffer();
}

// デストラクタ
Primitive::~Primitive()
{
	DescriptorMane::Get().DeleteRsc(rsc);
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
	desc.Width            = sizeof(prm::Vertex) * static_cast<size_t>(type);

	DescriptorMane::Get().CreateRsc(rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// マップ
long Primitive::Map(void)
{
	void* data = nullptr;

	auto hr = DescriptorMane::Get().GetRsc(rsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用頂点リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(prm::Vertex) * vertex.size());

	DescriptorMane::Get().GetRsc(rsc)->Unmap(0, nullptr);

	return hr;
}

// 描画
void Primitive::Draw(std::weak_ptr<List> list, const prm::Vertex * vertex, const size_t & num)
{
	this->vertex.assign(vertex, vertex + num);
	Map();

	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = DescriptorMane::Get().GetRsc(rsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(DescriptorMane::Get().GetRsc(rsc)->GetDesc().Width);
	view.StrideInBytes  = sizeof(prm::Vertex);
	list.lock()->SetVertexBufferView(view);

	auto heap = win.lock()->GetHeap();
	list.lock()->SetHeap(&heap, 1);
	list.lock()->SetRootTable(0, heap);

	list.lock()->SetTopology(topology[static_cast<int>(type)]);

	list.lock()->DrawVertex(this->vertex.size());
}
