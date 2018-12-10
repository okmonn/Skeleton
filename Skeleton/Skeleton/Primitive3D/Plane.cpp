#include "Plane.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Camera/Camera.h"
#include "../ShadowMap/ShadowMap.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// ���_��
#define VERTEX_MAX 4

// �R���X�g���N�^
Plane::Plane(std::weak_ptr<Device> dev, std::weak_ptr<Camera> cam, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe)
{
	this->dev = dev;
	this->cam = cam;
	this->root = root;
	this->pipe = pipe;

	vertex.resize(VERTEX_MAX);

	Init();
}

// �f�X�g���N
Plane::~Plane()
{
	descMane.DeleteRsc(constant);
	descMane.DeleteRsc(vRsc);
	descMane.DeleteHeap(constant);
}

// ������
void Plane::Init(void)
{
	descMane.CreateHeap(dev, constant, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	CreateRsc();
	CreateView();
	Map();
	CreateVertex();
}

// ���_�̃Z�b�g
long Plane::SetVertex(const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & size, const DirectX::XMFLOAT3 & color, const float & alpha)
{
	vertex[0] = { {pos.x,          pos.y,          pos.z},          { 0.0f, 1.0f }, { color.x, color.y, color.z, alpha } };
	vertex[1] = { {pos.x + size.x, pos.y,          pos.z},          { 1.0f, 1.0f }, { color.x, color.y, color.z, alpha } };
	vertex[2] = { {pos.x,          pos.y + size.y, pos.z + size.z}, { 0.0f, 0.0f }, { color.x, color.y, color.z, alpha } };
	vertex[3] = { {pos.x + size.x, pos.y + size.y, pos.z + size.z}, { 1.0f, 0.0f }, { color.x, color.y, color.z, alpha } };

	void* data = nullptr;

	auto hr = descMane.GetRsc(vRsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�|�C���g�p���_���\�[�X�̃}�b�v�F���s\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(prm3d::Vertex) * vertex.size());
	descMane.GetRsc(vRsc)->Unmap(0, nullptr);

	return hr;
}

// �`��
void Plane::Draw(std::weak_ptr<List> list, std::weak_ptr<ShadowMap>shadow)
{
	list.lock()->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	auto h = descMane.GetHeap(shadow.lock()->GetShaderHeap());
	list.lock()->GetList()->SetDescriptorHeaps(1, &h);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(0, h->GetGPUDescriptorHandleForHeapStart());

	h = descMane.GetHeap(constant);
	list.lock()->GetList()->SetDescriptorHeaps(1, &h);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(1, h->GetGPUDescriptorHandleForHeapStart());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(prm3d::Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(prm3d::Vertex);
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);
}
