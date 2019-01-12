#include "PrimitiveMane.h"
#include "Primitive.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// トポロジータイプ
const D3D12_PRIMITIVE_TOPOLOGY topology[] = {
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, 
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST, 
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST, 
	D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};

// コンストラクタ
PrimitiveMane::PrimitiveMane(std::weak_ptr<Window> win, std::weak_ptr<Root> root) : win(win), root(root)
{
	origin.clear();
	primitive.clear();

	Origin();
}

// デストラクタ
PrimitiveMane::~PrimitiveMane()
{
}

// オリジンのセット
void PrimitiveMane::Origin(void)
{
	origin[prm::PrimitiveType::point]    = std::make_unique<Primitive>(prm::PrimitiveType::point);
	origin[prm::PrimitiveType::line]     = std::make_unique<Primitive>(prm::PrimitiveType::line);
	origin[prm::PrimitiveType::triangle] = std::make_unique<Primitive>(prm::PrimitiveType::triangle);
}

// マップ
long PrimitiveMane::Map(Copy & copy)
{
	void* data = nullptr;
	auto hr = copy.rsc->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブのマップ：失敗\n"));
		return hr;
	}

	memcpy(data, copy.vertex.data(), sizeof(prm::Vertex) * copy.vertex.size());

	copy.rsc->Unmap(0, nullptr);

	return hr;
}

// 描画
void PrimitiveMane::Draw(std::weak_ptr<List> list, std::weak_ptr<Pipe> pipe, const prm::PrimitiveType & type, prm::Vertex * vertex, const size_t & num)
{
	primitive.push_back({ origin[type]->GetRsc(), std::vector<prm::Vertex>(vertex, vertex + num) });
	Map(primitive.back());

	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	auto heap = win.lock()->GetHeap();
	list.lock()->SetHeap(&heap, 1);
	list.lock()->SetRootTable(0, heap);

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = primitive.back().rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(sizeof(prm::Vertex) * primitive.back().vertex.size());
	view.StrideInBytes  = sizeof(prm::Vertex);
	list.lock()->SetVertexBufferView(view);

	list.lock()->SetTopology(topology[primitive.back().vertex.size()]);

	list.lock()->DrawVertex(primitive.back().vertex.size());

}
