#include "Pmd.h"
#include "PmdLoader.h"
#include "../Texture/TextureLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Camera/Camera.h"
#include "../Light/Light.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// アンマップ
#define UnMap(X) { if((X) != nullptr) (X)->Unmap(0, nullptr);  }

// コンストラクタ
Pmd::Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Light>light, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	loader(PmdLoader::Get()), tex(TextureLoader::Get()), descMane(DescriptorMane::Get()), 
	dev(dev), cam(cam), light(light), root(root), pipe(pipe), index(0)
{
	data.clear();
}

// デストラクタ
Pmd::~Pmd()
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		UnMap(descMane.GetRsc(itr->second.cRsc));
		UnMap(descMane.GetRsc(itr->second.mRsc));
		descMane.DeleteRsc(itr->second.cRsc);
		descMane.DeleteRsc(itr->second.mRsc);

		descMane.DeleteHeap(*itr->first);
	}
}

// シェーダービューの生成
void Pmd::CreateShaderView(const std::string & fileName, int * i, const int & index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = tex.GetRsc(fileName)->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto handle = descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
	dev.lock()->Get()->CreateShaderResourceView(tex.GetRsc(fileName), &desc, handle);
	this->index++;
}

// サブリソースに書き込み
long Pmd::WriteSub(const std::string & fileName)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = tex.GetRsc(fileName)->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex.GetRsc(fileName)->GetDesc().Width);
	box.top    = 0;

	auto hr = tex.GetRsc(fileName)->WriteToSubresource(0, &box, &tex.GetDecode(fileName)[0], tex.GetSub(fileName)->RowPitch, static_cast<UINT>(tex.GetSub(fileName)->SlicePitch));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nサブリソースの更新：失敗"));
	}

	return hr;
}

// 定数リソースの生成
long Pmd::CreateConRsc(int * i, int & rsc, const unsigned __int64 & size)
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
	desc.Width            = size;

	rsc = index++;

	return descMane.CreateRsc(dev, rsc, prop, desc);
}

// 定数ビューの生成
void Pmd::CreateConView(int * i, int & rsc, const unsigned int & size, const int& index)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(rsc)->GetGPUVirtualAddress() + (size * index);
	desc.SizeInBytes    = size;

	auto handle = descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * (rsc + index);

	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// マップ
long Pmd::Map(int & rsc, void ** data)
{
	auto hr = descMane.GetRsc(rsc)->Map(0, nullptr, data);
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
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + (size * data[i].cRsc);
	data[i].list->GetList()->SetGraphicsRootDescriptorTable(4, handle);

	//頂点のセット
	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(loader.GetVertexRsc(fileName))->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(pmd::Vertex) * loader.GetVertex(fileName).size();
	view.StrideInBytes  = sizeof(pmd::Vertex);
	data[i].list->GetList()->IASetVertexBuffers(0, 1, &view);

	//インデックスのセット
	D3D12_INDEX_BUFFER_VIEW iView{};
	iView.BufferLocation = descMane.GetRsc(loader.GetIndexRsc(fileName))->GetGPUVirtualAddress();
	iView.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;;
	iView.SizeInBytes    = sizeof(unsigned short) * loader.GetIndex(fileName).size();
	data[i].list->GetList()->IASetIndexBuffer(&iView);

	data[i].list->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//送信データ
	unsigned __int8* d = data[i].materialData;
	//オフセット
	unsigned int offset = 0;
	//テクスチャ番号
	int texIndex = 0;
	//加算テクスチャ番号
	int spaIndex = 0;
	//乗算テクスチャ番号
	int sphIndex = 0;

	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + (size * data[i].mRsc);
	for (unsigned int n = 0; n < loader.GetMaterial(fileName).size(); ++n)
	{
		data[i].mat.diffuse     = loader.GetMaterial(fileName)[n].diffuse;
		data[i].mat.alpha       = loader.GetMaterial(fileName)[n].alpha;
		data[i].mat.specularity = loader.GetMaterial(fileName)[n].specularity;
		data[i].mat.specula     = loader.GetMaterial(fileName)[n].specula;
		data[i].mat.mirror      = loader.GetMaterial(fileName)[n].mirror;
		data[i].mat.tex         = false;
		data[i].mat.spa			= false;
		data[i].mat.sph			= false;

		if (loader.GetTexture(fileName).find(n) != loader.GetTexture(fileName).end())
		{
			data[i].mat.tex = true;
			auto texHandle = heap->GetGPUDescriptorHandleForHeapStart();
			texHandle.ptr += size * texIndex;
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(0, texHandle);
		}
		else
		{
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetSpa(fileName).find(n) != loader.GetSpa(fileName).end())
		{
			data[i].mat.spa = true;
			auto spaHandle = heap->GetGPUDescriptorHandleForHeapStart();
			spaHandle.ptr += size * (loader.GetTexture(fileName).size() + spaIndex++);
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(1, spaHandle);
		}
		else
		{
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(1, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetSph(fileName).find(n) != loader.GetSph(fileName).end())
		{
			data[i].mat.sph = true;
			auto sphHandle = heap->GetGPUDescriptorHandleForHeapStart();
			sphHandle.ptr += size * (loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + sphIndex++);
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(2, sphHandle);
		}
		else
		{
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(2, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetToon(fileName).find(n) != loader.GetToon(fileName).end())
		{
			auto toonHandle = heap->GetGPUDescriptorHandleForHeapStart();
			toonHandle.ptr += size * (loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + loader.GetSph(fileName).size() + loader.GetMaterial(fileName)[n].toonIndex);
			data[i].list->GetList()->SetGraphicsRootDescriptorTable(3, toonHandle);
		}

		memcpy(d, &data[i].mat, sizeof(pmd::Mat));

		data[i].list->GetList()->SetGraphicsRootDescriptorTable(5, handle);

		data[i].list->GetList()->DrawIndexedInstanced(loader.GetMaterial(fileName)[n].indexNum, 1, offset, 0, 0);

		handle.ptr += size;

		d = (unsigned __int8*)(((sizeof(pmd::Mat) + 0xff) &~0xff) + (char*)(d));

		offset += loader.GetMaterial(fileName)[n].indexNum;
	}

	data[i].list->GetList()->Close();
}

// 読み込み
void Pmd::Load(const std::string & fileName, int & i)
{
	loader.Load(dev, fileName);

	data[&i].list   = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
	data[&i].cRsc   = 0;

	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + loader.GetSph(fileName).size() + 
		loader.GetToon(fileName).size() + 1 + loader.GetMaterial(fileName).size());

	//テクスチャ
	for (auto& n : loader.GetTexture(fileName))
	{
		CreateShaderView(n.second, &i, index);
		WriteSub(n.second);
	}
	for (auto& n : loader.GetSpa(fileName))
	{
		CreateShaderView(n.second, &i, index);
		WriteSub(n.second);
	}
	for (auto& n : loader.GetSph(fileName))
	{
		CreateShaderView(n.second, &i, index);
		WriteSub(n.second);
	}
	for (auto& n : loader.GetToon(fileName))
	{
		CreateShaderView(n.second, &i, index);
		WriteSub(n.second);
	}

	//WVP
	CreateConRsc(&i, data[&i].cRsc, (sizeof(WVP) + 0xff) &~0xff);
	CreateConView(&i, data[&i].cRsc, (sizeof(WVP) + 0xff) &~0xff);
	Map(data[&i].cRsc, reinterpret_cast<void**>(&data[&i].wvp));

	//マテリアル
	CreateConRsc(&i, data[&i].mRsc, ((sizeof(pmd::Mat) + 0xff) &~0xff) * loader.GetMaterial(fileName).size());
	for (unsigned int n = 0; n < loader.GetMaterial(fileName).size(); ++n)
	{
		CreateConView(&i, data[&i].mRsc, (sizeof(pmd::Mat) + 0xff) &~0xff, n);
	}
	Map(data[&i].mRsc, reinterpret_cast<void**>(&data[&i].materialData));

	Bundle(fileName, &i);

	DirectX::XMFLOAT4X4 tmp{};
	DirectX::XMStoreFloat4x4(&tmp, DirectX::XMMatrixIdentity());
	data[&i].wvp->world      = tmp;
	data[&i].wvp->view       = cam.lock()->GetView();
	data[&i].wvp->projection = cam.lock()->GetProjection();
	data[&i].wvp->eyePos     = cam.lock()->GetEye();
	data[&i].wvp->lightPos   = cam.lock()->GetLightPos();
	data[&i].wvp->lightViewProje = cam.lock()->GetLightViewProje();
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
	data[&i].wvp->view     = cam.lock()->GetView();
	data[&i].wvp->eyePos   = cam.lock()->GetEye();
	data[&i].wvp->lightPos = cam.lock()->GetLightPos();
	data[&i].wvp->lightViewProje = cam.lock()->GetLightViewProje();

	auto heap = descMane.GetHeap(i);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->ExecuteBundle(data[&i].list->GetList());
}
