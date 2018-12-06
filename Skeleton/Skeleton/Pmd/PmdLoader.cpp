#include "PmdLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../etc/Release.h"

// コンストラクタ
PmdLoader::PmdLoader() : 
	descMane(DescriptorMane::Get())
{
	data.clear();
}

// デストラクタ
PmdLoader::~PmdLoader()
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		descMane.DeleteRsc(itr->second.vRsc);
	}
}

// 頂点リソースの生成
long PmdLoader::CreateRsc(std::weak_ptr<Device>dev, int * i, const unsigned long long & size)
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

	return descMane.CreateRsc(dev, *i, prop, desc);
}

// マップ
template<typename T>
long PmdLoader::Map(int * i, T * data, const unsigned int & size, void ** rscData)
{
	auto hr = descMane.GetRsc(*i)->Map(0, nullptr, rscData);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDの頂点リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(*rscData, data, sizeof(data[0]) * size);

	descMane.GetRsc(*i)->Unmap(0, nullptr);

	return hr;
}

// 読み込み
int PmdLoader::Load(std::weak_ptr<Device>dev, const std::string & fileName)
{
	if (data.find(fileName) != data.end())
	{
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMDのファイル参照：失敗\n"));
		return -1;
	}

	//ヘッダー読み込み
	pmd::Header header = {};
	fread(&header, sizeof(header), 1, file);

	unsigned int num = 0;

	//頂点の読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].vertex.resize(num);
	for (auto itr = data[fileName].vertex.begin(); itr != data[fileName].vertex.end(); ++itr)
	{
		fread(&itr->pos,    sizeof(itr->pos),    1, file);
		fread(&itr->normal, sizeof(itr->normal), 1, file);
		fread(&itr->uv,     sizeof(itr->uv),     1, file);
		fread(&itr->bornNo, sizeof(itr->bornNo), 1, file);
		fread(&itr->weight, sizeof(itr->weight), 1, file);
		fread(&itr->edge,   sizeof(itr->edge),   1, file);
	}

	//頂点インデックス読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].index.resize(num);
	for (auto itr = data[fileName].index.begin(); itr != data[fileName].index.end(); ++itr)
	{
		fread(&(*itr), sizeof(unsigned short), 1, file);
	}

	//マテリアルの読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].material.resize(num);
	fread(data[fileName].material.data(), sizeof(pmd::Material), num, file);

	fclose(file);

	//頂点バッファ生成
	CreateRsc(dev, &data[fileName].vRsc, sizeof(pmd::Vertex) * data[fileName].vertex.size());
	Map(&data[fileName].vRsc, data[fileName].vertex.data(), data[fileName].vertex.size(), &data[fileName].vertexData);

	//インデックスバッファの生成
	CreateRsc(dev, &data[fileName].iRsc, sizeof(unsigned short) * data[fileName].index.size());
	Map(&data[fileName].iRsc, data[fileName].index.data(), data[fileName].index.size(), &data[fileName].indexData);

	return 0;
}
