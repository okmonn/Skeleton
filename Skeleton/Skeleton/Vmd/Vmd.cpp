#include "Vmd.h"
#include <algorithm>
#include <Windows.h>
#include <tchar.h>

struct MotionData {
	//ボーン名
	char name[15];
	//フレーム番号
	unsigned long flam;
	//位置
	DirectX::XMFLOAT3 pos;
	//回転
	DirectX::XMFLOAT4 rotation;
	//補完
	std::vector<unsigned char>inter;
};

// コンストラクタ
Vmd::Vmd()
{
	motion.clear();
	animTime.clear();
}

// デストラクタ
Vmd::~Vmd()
{
}

// 読み込み
int Vmd::Load(const std::string & fileName)
{
	if (motion.find(fileName) != motion.end())
	{
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nVMDの読み込み：失敗\n"));
		return -1;
	}

	fseek(file, 50, SEEK_SET);

	unsigned int num = 0;
	fread(&num, sizeof(unsigned int), 1, file);

	//モーション
	MotionData m = {};
	m.inter.resize(64);

	motion[fileName] = std::make_shared<std::map<std::string, std::vector<vmd::Motion>>>();

	for (unsigned int i = 0; i < num; ++i)
	{
		fread(&m.name, sizeof(m.name), 1, file);
		fread(&m.flam, sizeof(m.flam), 1, file);
		fread(&m.pos, sizeof(m.pos), 1, file);
		fread(&m.rotation, sizeof(m.rotation), 1, file);
		fread(&m.inter[0], sizeof(unsigned char) * m.inter.size(), 1, file);
		if (motion[fileName]->find(m.name) == motion[fileName]->end())
		{
			motion[fileName]->insert(std::make_pair(m.name, std::vector<vmd::Motion>()));
		}

		motion[fileName]->at(m.name).push_back({ m.flam, m.pos, m.rotation,
			DirectX::XMFLOAT2((float)m.inter[48] / 127.0f, (float)m.inter[52] / 127.0f),
			DirectX::XMFLOAT2((float)m.inter[56] / 127.0f, (float)m.inter[60] / 1270.f) });
	}

	fclose(file);

	for (auto itr = motion[fileName]->begin(); itr != motion[fileName]->end(); ++itr)
	{
		std::sort(itr->second.begin(), itr->second.end(),
			[](vmd::Motion& m1, vmd::Motion& m2) {
			return m1.flam < m2.flam;
		});

		animTime[fileName] = max((float)itr->second.back().flam, animTime[fileName]);
	}

	return 0;
}
