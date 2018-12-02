#include "Line.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// 頂点数
#define VERTEX_MAX 2

// コンストラクタ
Line::Line(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe)
{
	this->win = win;
	this->dev = dev;
	this->root = root;
	this->pipe = pipe;

	vertex.resize(VERTEX_MAX);

	list = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);

	Init();
}

// デストラクタ
Line::~Line()
{
	descMane.DeleteRsc(cRsc);
	descMane.DeleteRsc(vRsc);
	descMane.DeleteHeap(heap);
}

// 初期化
void Line::Init(void)
{
	descMane.CreateHeap(dev, heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	CreateRsc();
	CreateView();
	Map();
	CreateVrsc();
}

// バンドルのセット
void Line::Bundle(void)
{
	list->Reset(pipe.lock()->Get());

	list->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list->GetList()->SetPipelineState(pipe.lock()->Get());

	auto h = descMane.GetHeap(heap);
	list->GetList()->SetDescriptorHeaps(1, &h);
	list->GetList()->SetGraphicsRootDescriptorTable(0, h->GetGPUDescriptorHandleForHeapStart());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(prm::Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(prm::Vertex);
	list->GetList()->IASetVertexBuffers(0, 1, &view);

	list->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	list->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);

	list->GetList()->Close();
}

// 頂点のセット
long Line::SetVertex(const DirectX::XMFLOAT2 & pos1, const DirectX::XMFLOAT2 & pos2, const DirectX::XMFLOAT3 & color, const float & alpha)
{
	vertex[0] = { { pos1.x, pos1.y, 0.0f }, { color.x, color.y, color.z, alpha } };
	vertex[1] = { { pos2.x, pos2.y, 0.0f }, { color.x, color.y, color.z, alpha } };

	void* data = nullptr;

	auto hr = descMane.GetRsc(vRsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nポイント用頂点リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(prm::Vertex) * vertex.size());
	descMane.GetRsc(vRsc)->Unmap(0, nullptr);

	Bundle();

	return hr;
}

// 描画
void Line::Draw(std::weak_ptr<List> list)
{
	auto h = descMane.GetHeap(heap);
	list.lock()->GetList()->SetDescriptorHeaps(1, &h);
	list.lock()->GetList()->ExecuteBundle(this->list->GetList());
}
