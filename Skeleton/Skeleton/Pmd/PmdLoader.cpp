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
}

// 読み込み
int PmdLoader::Load(std::weak_ptr<Device>dev, const std::string & fileName)
{
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
	data[fileName].vertex = std::make_shared<std::vector<pmd::Vertex>>(num);
	for (auto itr = data[fileName].vertex->begin(); itr != data[fileName].vertex->end(); ++itr)
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
	data[fileName].index = std::make_shared<std::vector<unsigned short>>(num);
	for (auto itr = data[fileName].index->begin(); itr != data[fileName].index->end(); ++itr)
	{
		fread(&(*itr), sizeof(unsigned short), 1, file);
	}

	fclose(file);

	return 0;
}
