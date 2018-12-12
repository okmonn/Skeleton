#include "Pmd.h"
#include "PmdLoader.h"
#include "../Texture/TextureLoader.h"
#include "../Vmd/Vmd.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Camera/Camera.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../CurlNoise/CurlNoise.h"
#include "../etc/Release.h"
#include "../etc/Func.h"
#include <algorithm>

// アンマップ
#define UnMap(X) { if((X) != nullptr) (X)->Unmap(0, nullptr);  }

// コンストラクタ
Pmd::Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, 
	std::weak_ptr<Root>sRoot, std::weak_ptr<Pipe>sPipe) :
	loader(PmdLoader::Get()), tex(TextureLoader::Get()), motion(Vmd::Get()), descMane(DescriptorMane::Get()), 
	dev(dev), cam(cam), root(root), pipe(pipe), sRoot(sRoot), sPipe(sPipe), index(0)
{
	data.clear();

	curl = std::make_unique<CurlNoise>(dev, L"Shader/CurlNoise.hlsl");
}

// デストラクタ
Pmd::~Pmd()
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		UnMap(descMane.GetRsc(itr->second.cRsc));
		UnMap(descMane.GetRsc(itr->second.mRsc));
		UnMap(descMane.GetRsc(itr->second.bRsc));
		descMane.DeleteRsc(itr->second.cRsc);
		descMane.DeleteRsc(itr->second.mRsc);
		descMane.DeleteRsc(itr->second.bRsc);

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

	//ボーンのセット
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + (size * data[i].bRsc);
	data[i].list->GetList()->SetGraphicsRootDescriptorTable(6, handle);

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

// 影用バンドルのセット
void Pmd::ShadowBundle(const std::string & fileName, int * i)
{
	data[i].sList->Reset(pipe.lock()->Get());

	data[i].sList->GetList()->SetGraphicsRootSignature(sRoot.lock()->Get());
	data[i].sList->GetList()->SetPipelineState(sPipe.lock()->Get());

	auto heap = descMane.GetHeap(*i);
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	auto size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	data[i].sList->GetList()->SetDescriptorHeaps(1, &heap);

	//WVPのセット
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + (size * data[i].cRsc);
	data[i].sList->GetList()->SetGraphicsRootDescriptorTable(4, handle);

	//ボーンのセット
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + (size * data[i].bRsc);
	data[i].sList->GetList()->SetGraphicsRootDescriptorTable(6, handle);

	//頂点のセット
	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(loader.GetVertexRsc(fileName))->GetGPUVirtualAddress();
	view.SizeInBytes = sizeof(pmd::Vertex) * loader.GetVertex(fileName).size();
	view.StrideInBytes = sizeof(pmd::Vertex);
	data[i].sList->GetList()->IASetVertexBuffers(0, 1, &view);

	//インデックスのセット
	D3D12_INDEX_BUFFER_VIEW iView{};
	iView.BufferLocation = descMane.GetRsc(loader.GetIndexRsc(fileName))->GetGPUVirtualAddress();
	iView.Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;;
	iView.SizeInBytes = sizeof(unsigned short) * loader.GetIndex(fileName).size();
	data[i].sList->GetList()->IASetIndexBuffer(&iView);

	data[i].sList->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
		data[i].mat.diffuse = loader.GetMaterial(fileName)[n].diffuse;
		data[i].mat.alpha = loader.GetMaterial(fileName)[n].alpha;
		data[i].mat.specularity = loader.GetMaterial(fileName)[n].specularity;
		data[i].mat.specula = loader.GetMaterial(fileName)[n].specula;
		data[i].mat.mirror = loader.GetMaterial(fileName)[n].mirror;
		data[i].mat.tex = false;
		data[i].mat.spa = false;
		data[i].mat.sph = false;

		if (loader.GetTexture(fileName).find(n) != loader.GetTexture(fileName).end())
		{
			data[i].mat.tex = true;
			auto texHandle = heap->GetGPUDescriptorHandleForHeapStart();
			texHandle.ptr += size * texIndex;
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(0, texHandle);
		}
		else
		{
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetSpa(fileName).find(n) != loader.GetSpa(fileName).end())
		{
			data[i].mat.spa = true;
			auto spaHandle = heap->GetGPUDescriptorHandleForHeapStart();
			spaHandle.ptr += size * (loader.GetTexture(fileName).size() + spaIndex++);
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(1, spaHandle);
		}
		else
		{
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(1, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetSph(fileName).find(n) != loader.GetSph(fileName).end())
		{
			data[i].mat.sph = true;
			auto sphHandle = heap->GetGPUDescriptorHandleForHeapStart();
			sphHandle.ptr += size * (loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + sphIndex++);
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(2, sphHandle);
		}
		else
		{
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(2, heap->GetGPUDescriptorHandleForHeapStart());
		}
		if (loader.GetToon(fileName).find(n) != loader.GetToon(fileName).end())
		{
			auto toonHandle = heap->GetGPUDescriptorHandleForHeapStart();
			toonHandle.ptr += size * (loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + loader.GetSph(fileName).size() + loader.GetMaterial(fileName)[n].toonIndex);
			data[i].sList->GetList()->SetGraphicsRootDescriptorTable(3, toonHandle);
		}

		memcpy(d, &data[i].mat, sizeof(pmd::Mat));

		data[i].sList->GetList()->SetGraphicsRootDescriptorTable(5, handle);

		data[i].sList->GetList()->DrawIndexedInstanced(loader.GetMaterial(fileName)[n].indexNum, 1, offset, 0, 0);

		handle.ptr += size;

		d = (unsigned __int8*)(((sizeof(pmd::Mat) + 0xff) &~0xff) + (char*)(d));

		offset += loader.GetMaterial(fileName)[n].indexNum;
	}

	data[i].sList->GetList()->Close();
}

// 読み込み
void Pmd::Load(const std::string & fileName, int & i)
{
	loader.Load(dev, fileName);

	data[&i].model  = fileName;
	data[&i].list   = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
	data[&i].sList  = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
	data[&i].cRsc   = 0;
	
	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		loader.GetTexture(fileName).size() + loader.GetSpa(fileName).size() + loader.GetSph(fileName).size() + 
		loader.GetToon(fileName).size() + 1 + loader.GetMaterial(fileName).size() + 1);

	std::vector<DirectX::XMFLOAT3>pos;
	for (auto& n : loader.GetVertex(fileName))
	{
		pos.push_back(n.pos);
	}
	curl->UAV("u0", sizeof(DirectX::XMFLOAT3), pos.size());
	curl->Init(pos);

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

	//ボーン
	for (auto& n : loader.GetBorn(fileName))
	{
		data[&i].mtx.push_back(DirectX::XMMatrixIdentity());
	}
	CreateConRsc(&i, data[&i].bRsc, (sizeof(DirectX::XMMATRIX) * loader.GetBorn(fileName).size() + 0xff) &~0xff);
	CreateConView(&i, data[&i].bRsc, (sizeof(DirectX::XMMATRIX) * loader.GetBorn(fileName).size() + 0xff) &~0xff);
	Map(data[&i].bRsc, &data[&i].bornData);
	memcpy(data[&i].bornData, &data[&i].mtx[0], (sizeof(DirectX::XMMATRIX) * loader.GetBorn(fileName).size() + 0xff) &~0xff);

	//マテリアル
	CreateConRsc(&i, data[&i].mRsc, ((sizeof(pmd::Mat) + 0xff) &~0xff) * loader.GetMaterial(fileName).size());
	for (unsigned int n = 0; n < loader.GetMaterial(fileName).size(); ++n)
	{
		CreateConView(&i, data[&i].mRsc, (sizeof(pmd::Mat) + 0xff) &~0xff, n);
	}
	Map(data[&i].mRsc, reinterpret_cast<void**>(&data[&i].materialData));

	Bundle(fileName, &i);
	ShadowBundle(fileName, &i);

	DirectX::XMFLOAT4X4 tmp{};
	DirectX::XMStoreFloat4x4(&tmp, DirectX::XMMatrixIdentity());
	data[&i].wvp->world           = tmp;
	data[&i].wvp->view            = cam.lock()->GetView();
	data[&i].wvp->projection      = cam.lock()->GetProjection();
	data[&i].wvp->lightView       = cam.lock()->GetLightView();
	data[&i].wvp->lightProjection = cam.lock()->GetLightProjection();
	data[&i].wvp->eyePos          = cam.lock()->GetEye();
	data[&i].wvp->lightPos        = cam.lock()->GetLightPos();
}

// モーションの適応
void Pmd::Attach(const std::string & fileName, int & i)
{
	if (motion.Load(fileName) == 0)
	{
		data[&i].motion   = motion.GetMotion(fileName);
		data[&i].flam     = 0.0f;
		data[&i].animTime = motion.GetAnimTime(fileName);
	}
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
	curl->Execution();

	data[&i].wvp->view      = cam.lock()->GetView();
	data[&i].wvp->lightView = cam.lock()->GetLightView();
	data[&i].wvp->eyePos    = cam.lock()->GetEye();
	data[&i].wvp->lightPos  = cam.lock()->GetLightPos();

	auto heap = descMane.GetHeap(i);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->ExecuteBundle(data[&i].list->GetList());
}

// 影描画
void Pmd::DrawShadow(std::weak_ptr<List> list, int & i)
{
	data[&i].wvp->view = cam.lock()->GetView();
	data[&i].wvp->lightView = cam.lock()->GetLightView();
	data[&i].wvp->eyePos = cam.lock()->GetEye();
	data[&i].wvp->lightPos = cam.lock()->GetLightPos();

	auto heap = descMane.GetHeap(i);
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);
	list.lock()->GetList()->ExecuteBundle(data[&i].sList->GetList());
}

// ボーンの回転
void Pmd::RotateBorn(int * i, const std::string & name, const DirectX::XMMATRIX & mtx)
{
	auto& node = loader.GetBornNode(data[i].model);

	if (node.find(name) == node.end())
	{
		return;
	}

	auto vec = DirectX::XMLoadFloat3(&node[name].start);

	data[i].mtx[node[name].index] = DirectX::XMMatrixTranslationFromVector(
		DirectX::XMVectorScale(vec, -1.0f)) *
		mtx *
		DirectX::XMMatrixTranslationFromVector(vec);
}

// ボーンの再帰処理
void Pmd::RecursiveBorn(int * i, pmd::BornNode & node, const DirectX::XMMATRIX & mtx)
{
	data[i].mtx[node.index] *= mtx;

	for (auto& child : node.child)
	{
		RecursiveBorn(i, *child, data[i].mtx[node.index]);
	}
}

// アニメーションのリセット
void Pmd::ResetAnim(int * i)
{
	data[i].flam = 0.0f;

	std::fill(data[i].mtx.begin(), data[i].mtx.end(), DirectX::XMMatrixIdentity());

	for (auto itr = data[i].motion.lock()->begin(); itr != data[i].motion.lock()->end(); ++itr)
	{
		auto& key = itr->second;

		auto now = std::find_if(key.rbegin(), key.rend(),
			[&](const vmd::Motion& m) {return m.flam <= (unsigned int)data[i].flam; });
		if (now == key.rend())
		{
			continue;
		}
		auto nowVec = DirectX::XMLoadFloat4(&now->rotation);

		RotateBorn(i, itr->first, DirectX::XMMatrixRotationQuaternion(nowVec));
	}

	RecursiveBorn(i, loader.GetBornNode(data[i].model)["センター"], data[i].mtx[loader.GetBornNode(data[i].model)["センター"].index]);

	memcpy(data[i].bornData, data[i].mtx.data(), ((sizeof(DirectX::XMMATRIX) * data[i].mtx.size() + 0xff) &~0xff));
}

// アニメーション
void Pmd::Animation(int & i, const bool & loop, const float & animSpeed)
{
	std::fill(data[&i].mtx.begin(), data[&i].mtx.end(), DirectX::XMMatrixIdentity());

	DirectX::XMVECTOR nowVec{};
	float nowFlam = 0.0f;
	DirectX::XMVECTOR nexVec{};
	float nexFlam = 0.0f;

	float time = 0.0f;
	DirectX::XMMATRIX trans;

	for(auto itr = data[&i].motion.lock()->begin(); itr != data[&i].motion.lock()->end(); ++itr)
	{
		auto now = std::find_if(itr->second.rbegin(), itr->second.rend(),
			[&](const vmd::Motion& m) {return m.flam <= static_cast<unsigned long>(data[&i].flam); });
		if (now == itr->second.rend())
		{
			continue;
		}
		nowVec  = DirectX::XMLoadFloat4(&now->rotation);
		nowFlam = static_cast<float>(now->flam);

		if (now.base() == itr->second.end())
		{
			RotateBorn(&i, itr->first, DirectX::XMMatrixRotationQuaternion(nowVec));
		}
		else
		{
			nexVec  = DirectX::XMLoadFloat4(&now.base()->rotation);
			nexFlam = static_cast<float>(now.base()->flam);

			time = func::Newton((data[&i].flam - nowFlam) / (nexFlam - nowFlam), 
				now.base()->a.x, now.base()->a.y, now.base()->b.x, now.base()->b.y);

			trans = (DirectX::XMMatrixTranslation(now->pos.x, now->pos.y, now->pos.z) * (1.0f - time))
				+ (DirectX::XMMatrixTranslation(now.base()->pos.x, now.base()->pos.y, now.base()->pos.z) * time);

			RotateBorn(&i, itr->first, DirectX::XMMatrixRotationQuaternion(
				DirectX::XMQuaternionSlerp(nowVec, nexVec, time)) * trans);
		}
	}

	RecursiveBorn(&i, loader.GetBornNode(data[&i].model)["センター"], data[&i].mtx[loader.GetBornNode(data[&i].model)["センター"].index]);

	memcpy(data[&i].bornData, data[&i].mtx.data(), ((sizeof(DirectX::XMMATRIX) * data[&i].mtx.size() + 0xff) &~0xff));

	data[&i].flam += animSpeed;

	if (loop == true && CheckAnimEnd(i))
	{
		ResetAnim(&i);
	}
}

// アニメーション終了確認
bool Pmd::CheckAnimEnd(int & i)
{
	return (data[&i].flam >= data[&i].animTime + 10.0f);
}

// モデルの削除
void Pmd::Delete(int & i)
{
	if (data.find(&i) == data.end())
	{
		return;
	}

	UnMap(descMane.GetRsc(data[&i].cRsc));
	UnMap(descMane.GetRsc(data[&i].mRsc));
	UnMap(descMane.GetRsc(data[&i].bRsc));
	descMane.DeleteRsc(data[&i].cRsc);
	descMane.DeleteRsc(data[&i].mRsc);
	descMane.DeleteRsc(data[&i].bRsc);

	descMane.DeleteHeap(i);

	data.erase(data.find(&i));
}
