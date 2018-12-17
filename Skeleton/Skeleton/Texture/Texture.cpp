#include "Texture.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "TextureLoader.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// 頂点数
#define VERTEX_MAX 4

// リソースの数
#define RSC_NUM 2

// アンマップ
#define UnMap(X) { if((X) != nullptr) (X)->Unmap(0, nullptr);  }

// コンストラクタ
Texture::Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	descMane(DescriptorMane::Get()), loader(TextureLoader::Get()), win(win), dev(dev), root(root), pipe(pipe),
	vRsc(0), vertexData(nullptr)
{
	vertex.resize(VERTEX_MAX);

	tex.clear();

	SetVertex();
	CreateVertexRsc();
	MapVertex();
}

// デストラクタ
Texture::~Texture()
{
	for (auto itr = tex.begin(); itr != tex.end(); ++itr)
	{
		UnMap(descMane.GetRsc(itr->second.cRsc));
		descMane.DeleteRsc(itr->second.cRsc);
		descMane.DeleteHeap(*itr->first);
	}

	descMane.DeleteRsc(vRsc);
}

// 頂点のセット
void Texture::SetVertex(void)
{
	vertex[0] = { { 0.0f,                      0.0f,                     0.0f }, { 0.0f, 0.0f } };
	vertex[1] = { { (float)win.lock()->GetX(), 0.0f,                     0.0f }, { 1.0f, 0.0f } };
	vertex[2] = { { 0.0f,                     (float)win.lock()->GetY(), 0.0f }, { 0.0f, 1.0f } };
	vertex[3] = { {(float)win.lock()->GetX(), (float)win.lock()->GetY(), 0.0f }, { 1.0f, 1.0f } };
}

// 頂点リソースの生成
long Texture::CreateVertexRsc(void)
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
	desc.Width            = sizeof(tex::Vertex) * vertex.size();

	return descMane.CreateRsc(dev, vRsc, prop, desc);
}

// 頂点リソースのマップ
long Texture::MapVertex(void)
{
	auto hr = descMane.GetRsc(vRsc)->Map(0, nullptr, &vertexData);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの頂点リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(vertexData, vertex.data(), sizeof(tex::Vertex) * vertex.size());

	descMane.GetRsc(vRsc)->Unmap(0, nullptr);

	return hr;
}

// 定数リソースの生成
long Texture::CreateConRsc(int * i)
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
	desc.Width            = (sizeof(tex::Info) + 0xff) &~0xff;

	return  descMane.CreateRsc(dev, tex[i].cRsc, prop, desc);
}

// 定数バッファビューの生成
void Texture::CreateConView(int * i)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(tex[i].cRsc)->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(tex::Info) + 0xff) &~0xff;

	auto handle = descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart();
	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// 定数バッファのマップ
long Texture::MapCon(int * i)
{
	auto hr = descMane.GetRsc(tex[i].cRsc)->Map(0, nullptr, (void**)(&tex[i].info));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの定数バッファのマップ：失敗\n"));
		return hr;
	}

	tex[i].info->window = { static_cast<float>(win.lock()->GetX()), static_cast<float>(win.lock()->GetY()) };

	return hr;
}

// シェーダーリソースの生成
long Texture::CreateShaderRsc(int * i, const unsigned int & width, const unsigned int & height)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width            = width;
	desc.Height           = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc       = { 1, 0 };
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tex[i].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n作成テクスチャのリソース生成：失敗\n"));
	}

	return hr;
}

// シェーダーリソースビューの生成
void Texture::CreateShaderView(int * i)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = tex[i].rsc->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto handle = descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dev.lock()->Get()->CreateShaderResourceView(tex[i].rsc, &desc, handle);
}

// サブリソースに書き込み
long Texture::WriteSub(int * i)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = tex[i].rsc->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[i].rsc->GetDesc().Width);
	box.top    = 0;

	auto hr = tex[i].rsc->WriteToSubresource(0, &box, &tex[i].decode[0], static_cast<unsigned int>(tex[i].sub.lock()->RowPitch), static_cast<unsigned int>(tex[i].sub.lock()->SlicePitch));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nサブリソースの更新：失敗"));
	}

	return hr;
}

// 作成テクスチャのサブリソースに書き込み
long Texture::WriteSub(int * i, const std::vector<unsigned char>& data)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = tex[i].rsc->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[i].rsc->GetDesc().Width);
	box.top    = 0;

	auto hr = tex[i].rsc->WriteToSubresource(0, &box, &data[0], box.right * 4, box.right * box.bottom * 4);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n作成テクスチャのサブリソースの更新：失敗"));
	}

	return hr;
}

// バンドルのセット
void Texture::SetBundle(int * i)
{
	tex[i].list->Reset(pipe.lock()->Get());

	tex[i].list->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	tex[i].list->GetList()->SetPipelineState(pipe.lock()->Get());

	auto heap = descMane.GetHeap(*i);
	tex[i].list->GetList()->SetDescriptorHeaps(1, &heap);

	auto handle = descMane.GetHeap(*i)->GetGPUDescriptorHandleForHeapStart();
	tex[i].list->GetList()->SetGraphicsRootDescriptorTable(0, handle);
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<unsigned int>(sizeof(tex::Vertex) * vertex.size());
	view.StrideInBytes  = sizeof(tex::Vertex);
	tex[i].list->GetList()->IASetVertexBuffers(0, 1, &view);

	tex[i].list->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex[i].list->GetList()->SetGraphicsRootDescriptorTable(1, handle);
	tex[i].list->GetList()->DrawInstanced(static_cast<unsigned int>(vertex.size()), 1, 0, 0);

	tex[i].list->GetList()->Close();
}

// 読み込み
void Texture::Load(const std::string & fileName, int & i)
{
	if (FAILED(loader.Load(dev, fileName)))
	{
		return;
	}

	tex[&i].rsc    = loader.GetRsc(fileName);
	tex[&i].decode = loader.GetDecode(fileName);
	tex[&i].sub    = loader.GetSub(fileName);
	tex[&i].list   = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);

	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_NUM);
	CreateConRsc(&i);
	CreateConView(&i);
	MapCon(&i);
	CreateShaderView(&i);
	WriteSub(&i);
	SetBundle(&i);
}

// 配列データから画像を生成
void Texture::CreateImg(const std::vector<unsigned char>& data, const unsigned int & width, const unsigned int & height, int & i)
{
	CreateShaderRsc(&i, width, height);
	tex[&i].list = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_NUM);
	CreateConRsc(&i);
	CreateConView(&i);
	MapCon(&i);
	CreateShaderView(&i);
	WriteSub(&i, data);
	SetBundle(&i);
}

// 描画
void Texture::Draw(std::weak_ptr<List>list, int & i, const DirectX::XMFLOAT2 & pos, const DirectX::XMFLOAT2 & size,
	const DirectX::XMFLOAT2 & uvPos, const DirectX::XMFLOAT2 & uvSize, const float & alpha, const bool & turnX, const bool & turnY)
{
	XMStoreFloat4x4(&tex[&i].info->matrix,
		DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat2(
			&DirectX::XMFLOAT2(size.x / static_cast<float>(win.lock()->GetX()), size.y / static_cast<float>(win.lock()->GetY()))))
		* DirectX::XMMatrixTranslationFromVector(
			DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(pos.x, pos.y)))
	);
	tex[&i].info->uvPos   = uvPos;
	tex[&i].info->uvSize  = uvSize;
	tex[&i].info->reverse = { (turnX) ? 1.0f : 0.0f, (turnY) ? 1.0f : 0.0f };
	tex[&i].info->alpha   = alpha;

	auto heap = descMane.GetHeap(i);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->ExecuteBundle(tex[&i].list->GetList());
}

// 削除
void Texture::Delete(int & i)
{
	if (tex.find(&i) != tex.end())
	{
		UnMap(descMane.GetRsc(tex[&i].cRsc));
		descMane.DeleteRsc(tex[&i].cRsc);
		descMane.DeleteHeap(i);
		tex.erase(tex.find(&i));
	}
}
