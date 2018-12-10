#include "PmdLoader.h"
#include "../Texture/TextureLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../etc/Release.h"
#include "../etc/Func.h"
#include <iostream>

// コンストラクタ
PmdLoader::PmdLoader() : 
	tex(TextureLoader::Get()), descMane(DescriptorMane::Get())
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

// テクスチャの読み込み
void PmdLoader::LoadTex(std::weak_ptr<Device>dev, const std::string & fileName)
{
	for (unsigned int i = 0; i < data[fileName].material.size(); ++i)
	{
		if (data[fileName].material[i].texPath[0] == '\0')
		{
			continue;
		}

		std::string tmp = (char*)data[fileName].material[i].texPath;

		std::string pass;
		if (tmp.find("*") != std::string::npos)
		{
			auto tmp1 = tmp.substr(0, tmp.find_first_of('*'));
			pass = func::FindString(fileName, '/') + tmp1;
			tex.Load(dev, pass);
			data[fileName].tex.emplace(i, pass);

			tmp1 = tmp.substr(tmp.find_first_of('*') + 1, tmp.size());
			pass = func::FindString(fileName, '/') + tmp1;
			LoadSph(dev, fileName, pass, i);

			continue;
		}

		pass = func::FindString(fileName, '/') + tmp;
		if (pass.find(".spa") != std::string::npos)
		{
			LoadSpa(dev, fileName, pass, i);
			continue;
		}

		if (pass.find(".sph") != std::string::npos)
		{
			LoadSph(dev, fileName, pass, i);
			continue;
		}

		tex.Load(dev, pass);
		data[fileName].tex.emplace(i, pass);
	}
}

// 加算テクスチャの読み込み
void PmdLoader::LoadSpa(std::weak_ptr<Device> dev, const std::string & fileName, const std::string & pass, const unsigned int & index)
{
	tex.Load(dev, pass);
	data[fileName].spa.emplace(index, pass);
}

// 乗算テクスチャの読み込み
void PmdLoader::LoadSph(std::weak_ptr<Device> dev, const std::string & fileName, const std::string & pass, const unsigned int & index)
{
	tex.Load(dev, pass);
	data[fileName].sph.emplace(index, pass);
}

// トゥーンテクスチャの読み込み
void PmdLoader::LoadToon(std::weak_ptr<Device> dev, const std::string & fileName)
{
	for (unsigned int i = 0; i < toonName.size(); ++i)
	{
		std::string tmp = toonName[i];
		auto path = func::FindString(fileName, '/') + "toon/" + tmp;
		tex.Load(dev, path);
		data[fileName].toon.emplace(i, path);
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
		OutputDebugString(_T("\nPMDのマップ：失敗\n"));
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

	//ボーンの読み込み
	unsigned short bornNum = 0;
	fread(&bornNum, sizeof(unsigned short), 1, file);
	data[fileName].born.resize(bornNum);
	for (auto& i : data[fileName].born)
	{
		fread(&i.name,     sizeof(i.name),     1, file);
		fread(&i.pIndex,   sizeof(i.pIndex),   1, file);
		fread(&i.cIndex,   sizeof(i.cIndex),   1, file);
		fread(&i.type,     sizeof(i.type),     1, file);
		fread(&i.IKpIndex, sizeof(i.IKpIndex), 1, file);
		fread(&i.pos,      sizeof(i.pos),      1, file);
	}

	//IK 
	unsigned short ikNum = 0; fread(&ikNum, sizeof(ikNum), 1, file); //モチロンすっ飛ばす 
	for (int i = 0; i < ikNum; ++i) {
		fseek(file, 4, SEEK_CUR);  unsigned char ikchainNum;//ぶっ殺すぞ 
		fread(&ikchainNum, sizeof(ikchainNum), 1, file);
		fseek(file, 6, SEEK_CUR);  fseek(file, ikchainNum * sizeof(unsigned short), SEEK_CUR);
	}

	//表情 
	unsigned short skinNum = 0; fread(&skinNum, sizeof(skinNum), 1, file); //当然の権利のようにすっ飛ばすで 
	for (int i = 0; i < skinNum; ++i) {
		fseek(file, 20, SEEK_CUR);
		unsigned int vertNum = 0;
		fread(&vertNum, sizeof(vertNum), 1, file);
		fseek(file, 1, SEEK_CUR);  fseek(file, 16 * vertNum, SEEK_CUR);
	}

	//表示用表情 
	unsigned char skinDispNum = 0;
	fread(&skinDispNum, sizeof(skinDispNum), 1, file); 
	fseek(file, skinDispNum * sizeof(unsigned short), SEEK_CUR);

	//表示用ボーン名 
	unsigned char boneDispNum = 0;
	fread(&boneDispNum, sizeof(boneDispNum), 1, file); 
	fseek(file, 50 * boneDispNum, SEEK_CUR);

	//表示ボーンリスト 
	unsigned int dispBoneNum = 0;
	fread(&dispBoneNum, sizeof(dispBoneNum), 1, file);
	fseek(file, 3 * dispBoneNum, SEEK_CUR);

	//英名 //英名対応フラグ 
	unsigned char englishFlg = 0;
	fread(&englishFlg, sizeof(englishFlg), 1, file);
	if (englishFlg) {
		//モデル名20バイト+256バイトコメント 
		fseek(file, 20 + 256, SEEK_CUR);
		//ボーン名20バイト*ボーン数 
		fseek(file, bornNum * 20, SEEK_CUR);
		//(表情数-1)*20バイト。-1なのはベース部分ぶん 
		fseek(file, (skinNum - 1) * 20, SEEK_CUR);
		//ボーン数*50バイト。 
		fseek(file, boneDispNum * 50, SEEK_CUR);
	}

	fread(toonName.data(), sizeof(char) * 100, toonName.size(), file);

	fclose(file);

	//テクスチャ読み込み
	LoadTex(dev, fileName);
	LoadToon(dev, fileName);

	//頂点バッファ生成
	CreateRsc(dev, &data[fileName].vRsc, sizeof(pmd::Vertex) * data[fileName].vertex.size());
	Map(&data[fileName].vRsc, data[fileName].vertex.data(), data[fileName].vertex.size(), &data[fileName].vertexData);

	//インデックスバッファの生成
	CreateRsc(dev, &data[fileName].iRsc, sizeof(unsigned short) * data[fileName].index.size());
	Map(&data[fileName].iRsc, data[fileName].index.data(), data[fileName].index.size(), &data[fileName].indexData);

	return 0;
}
