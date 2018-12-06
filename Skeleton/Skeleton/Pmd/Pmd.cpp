#include "Pmd.h"
#include "PmdLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Camera/Camera.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// コンストラクタ
Pmd::Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	loader(PmdLoader::Get()), descMane(DescriptorMane::Get()), dev(dev), cam(cam), root(root), pipe(pipe)
{
	data.clear();
}

// デストラクタ
Pmd::~Pmd()
{
}

// 定数リソースの生成
long Pmd::CreateConRsc(int * i, int & rsc)
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
	desc.Width            = (sizeof(WVP) + 0xff) &~0xff;

	return descMane.CreateRsc(dev, rsc, prop, desc);
}

// 定数ビューの生成
void Pmd::CreateConView(int * i, int & rsc)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(rsc)->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	auto handle = descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc;

	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// マップ
long Pmd::Map(int * i, int & rsc)
{
	auto hr = descMane.GetRsc(rsc)->Map(0, nullptr, (void**)&data[i].wvp);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDのマップ：失敗\n"));
	}

	return hr;
}

// バンドルのセット
void Pmd::Bundle(const std::string & fileName, int * i)
{
	data[i].list->Reset(pipe.lock()->Get());

	data[i].list->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	data[i].list->GetList()->SetPipelineState(pipe.lock()->Get());

	auto heap   = descMane.GetHeap(*i);
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	auto size   = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	data[i].list->GetList()->SetDescriptorHeaps(1, &heap);

	//WVPのセット
	handle.ptr += size * data[i].cRsc;
	data[i].list->GetList()->SetGraphicsRootDescriptorTable(0, handle);

	//頂点のセット
	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(loader.GetVertexRsc(fileName))->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(pmd::Vertex) * data[i].vertex.lock()->size();
	view.StrideInBytes  = sizeof(pmd::Vertex);
	data[i].list->GetList()->IASetVertexBuffers(0, 1, &view);

	data[i].list->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	data[i].list->GetList()->DrawInstanced(data[i].vertex.lock()->size(), 1, 0, 0);

	data[i].list->GetList()->Close();
}

// 読み込み
void Pmd::Load(const std::string & fileName, int & i)
{
	loader.Load(dev, fileName);

	data[&i].list   = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
	data[&i].vertex = loader.GetVertex(fileName);
	data[&i].cRsc   = 0;

	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	CreateConRsc(&i, data[&i].cRsc);
	CreateConView(&i, data[&i].cRsc);
	Map(&i, data[&i].cRsc);
	Bundle(fileName, &i);

	DirectX::XMFLOAT4X4 tmp{};
	DirectX::XMStoreFloat4x4(&tmp, DirectX::XMMatrixIdentity());
	data[&i].wvp->world = tmp;
	data[&i].wvp->view = cam.lock()->GetView();
	data[&i].wvp->projection = cam.lock()->GetProjection();
}

// 回転
void Pmd::Rotate(int & i, const float & angle)
{
	DirectX::XMFLOAT4X4 tmp;
	DirectX::XMStoreFloat4x4(&tmp, DirectX::XMMatrixRotationY(RAD(angle)));

	data[&i].wvp->world = tmp;
}

// 描画
void Pmd::Draw(std::weak_ptr<List>list, int & i)
{
	data[&i].wvp->view = cam.lock()->GetView();

	auto heap = descMane.GetHeap(i);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->ExecuteBundle(data[&i].list->GetList());
}
