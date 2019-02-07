#include "Texture.h"
#include "TexLoader.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Release.h"

// ���\�[�X��
#define RSC_NUM 2

// �R���X�g���N�^
Texture::Texture(std::weak_ptr<Window> win, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe) : win(win), root(root), pipe(pipe),
	heap(nullptr), cRsc(nullptr), sRsc(nullptr), vRsc(nullptr), info(nullptr), index(0)
{
	Init();
}

// �f�X�g���N�^
Texture::~Texture()
{
	UnMap(cRsc);
	Release(vRsc);
	Release(cRsc);
	Release(heap);
}

// ���_���\�[�X�̐���
long Texture::CreateVRsc(void)
{
	tex::Vertex vertex[] = {
		{ { 0.0f,                           0.0f,                          0.0f }, { 0.0f, 0.0f } },
		{ { (float)win.lock()->GetSize().x, 0.0f,                          0.0f }, { 1.0f, 0.0f } },
		{ { 0.0f,                          (float)win.lock()->GetSize().y, 0.0f }, { 0.0f, 1.0f } },
		{ { (float)win.lock()->GetSize().x,(float)win.lock()->GetSize().y, 0.0f }, { 1.0f, 1.0f } }
	};

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
	desc.Width            = sizeof(tex::Vertex) * _countof(vertex);

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vRsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p���_���\�[�X�̐����F���s\n"));
		return hr;
	}

	void* data = nullptr;
	hr = vRsc->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p���_���\�[�X�̃}�b�v�F���s\n"));
		return hr;
	}

	memcpy(data, &vertex[0], sizeof(vertex));
	UnMap(vRsc);

	return hr;
}

// �萔�p�q�[�v�̐���
long Texture::CreateHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = RSC_NUM;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = Dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�萔�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// �萔���\�[�X�̐���
long Texture::CreateCRsc(void)
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
	desc.Width            = (sizeof(tex::CbvInfo) + 0xff) &~0xff;

	auto hr = Dev->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cRsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�萔���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// CBV�̐���
void Texture::CBV(void)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = cRsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = static_cast<unsigned int>(cRsc->GetDesc().Width);

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	Dev->CreateConstantBufferView(&desc, handle);

	++index;
}

// �}�b�v
long Texture::Map(void)
{
	auto hr = cRsc->Map(0, nullptr, reinterpret_cast<void**>(&info));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�萔���\�[�X�̃}�b�v�F���s\n"));
	}

	return hr;
}

// ������
void Texture::Init(void)
{
	CreateVRsc();
	CreateHeap();
	CreateCRsc();
	CBV();
	Map();
}

// SRV�̐���
void Texture::SRV(void)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                  = sRsc->GetDesc().Format;
	desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels     = 1;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	Dev->CreateShaderResourceView(sRsc, &desc, handle);
	++index;
}

// �T�u���\�[�X�ɏ�����
long Texture::WriteSub(const std::string & fileName)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = sRsc->GetDesc().Height;
	box.right  = static_cast<unsigned int>(sRsc->GetDesc().Width);

	std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub = TexLoader::Get().GetSub(fileName);
	auto hr = sRsc->WriteToSubresource(0, &box, &TexLoader::Get().GetDecode(fileName)[0], static_cast<unsigned int>(sub.lock()->RowPitch), static_cast<unsigned int>(sub.lock()->SlicePitch));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�V�F�[�_���\�[�X�̏����݁F���s\n"));
	}

	return hr;
}

// �ǂݍ���
void Texture::Load(const std::string & fileName)
{
	if (FAILED(TexLoader::Get().Load(fileName)))
	{
		return;
	}

	sRsc = TexLoader::Get().GetRsc(fileName);
	SRV();
	WriteSub(fileName);
}

// �`��̏���
void Texture::SetDraw(const DirectX::XMFLOAT2 & pos, const DirectX::XMFLOAT2 & size, const DirectX::XMFLOAT2 & uvPos, const DirectX::XMFLOAT2 & uvSize, 
	const float & alpha, const float & angle, const bool & turnX, const bool & turnY)
{
	DirectX::XMStoreFloat4x4(&info->matrix,
		DirectX::XMMatrixAffineTransformation2D(
			DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(size.x / static_cast<float>(win.lock()->GetSize().x), size.y / static_cast<float>(win.lock()->GetSize().y))),
			DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(size.x / 2.0f, size.y / 2.0f)),
			angle,
			DirectX::XMLoadFloat2(&pos)
		));

	info->uvPos   = uvPos;
	info->uvSize  = uvSize;
	info->reverse = { (turnX) ? 1.0f : 0.0f, (turnY) ? 1.0f : 0.0f };
	info->alpha   = alpha;
}

// �`��
void Texture::Draw(std::weak_ptr<List> list)
{
	list.lock()->Root(root.lock()->Get());
	list.lock()->Pipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = vRsc->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(vRsc->GetDesc().Width);
	view.StrideInBytes  = sizeof(tex::Vertex);
	list.lock()->VertexView(view);

	list.lock()->Heap(&heap, 1);
	for (int i = 0; i < RSC_NUM; ++i)
	{
		list.lock()->RootTable(i, heap, i);
	}

	auto heap = win.lock()->GetHeap();
	list.lock()->Heap(&heap, 1);
	list.lock()->RootTable(RSC_NUM, heap);

	list.lock()->Topology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	list.lock()->DrawVertex((view.SizeInBytes / view.StrideInBytes));
}
