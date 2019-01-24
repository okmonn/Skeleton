#include "Primitive.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Primitive::Primitive(std::weak_ptr<Window> win, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe, const size_t & num) : 
	win(win), root(root), pipe(pipe), rsc(0)
{
	VertexBuffer(num);
}

// �f�X�g���N�^
Primitive::~Primitive()
{
	DescriptorMane::Get().DeleteRsc(rsc);
}

// ���_�o�b�t�@�̐���
void Primitive::VertexBuffer(const size_t & num)
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
	desc.Width            = sizeof(prm::Vertex) * num;

	DescriptorMane::Get().CreateRsc(rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// �}�b�v
long Primitive::Map(const std::vector<prm::Vertex> & vertex)
{
	void* data = nullptr;

	auto hr = DescriptorMane::Get().GetRsc(rsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�v���~�e�B�u�p���_���\�[�X�̃}�b�v�F���s\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(prm::Vertex) * vertex.size());

	DescriptorMane::Get().GetRsc(rsc)->Unmap(0, nullptr);

	return hr;
}

// �`��
void Primitive::Draw(std::weak_ptr<List> list, const std::vector<prm::Vertex> & vertex, const D3D_PRIMITIVE_TOPOLOGY & type)
{
	Map(vertex);

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

	list.lock()->SetTopology(type);

	list.lock()->DrawVertex(vertex.size());
}
