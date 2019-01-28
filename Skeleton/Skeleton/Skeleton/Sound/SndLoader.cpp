#include "SndLoader.h"
#include "../Useful/Useful.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ
SndLoader::SndLoader()
{
	info.clear();
	data.clear();

	Init();
}

// デストラクタ
SndLoader::~SndLoader()
{
}

// 初期化
void SndLoader::Init(void)
{
	load[8] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadWave8(tmp, file);
	};

	load[16] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadWave16(tmp, file);
	};
}

// 読み込み
int SndLoader::Load(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		OutputDebugString(_T("\nサウンドファイル読み込み済み\n"));
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nファイルの参照：失敗\n"));
		return -1;
	}

	snd::RIFF riff{};
	snd::FMT fmt{};
	snd::DATA data{};

	snd::LoadRIFF(riff, file);
	snd::LoadFMT(fmt,   file);
	snd::LoadDATA(data, file);

	info[fileName].bit     = fmt.bit;
	info[fileName].channel = fmt.channel;
	info[fileName].sample  = fmt.sample;

	this->data[fileName] = std::make_shared<std::vector<float>>(data.size / use::Byte(fmt.bit));
	load[fmt.bit](*this->data[fileName], file);

	return 0;
}

// サウンド情報の取得
snd::Info SndLoader::GetInfo(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return snd::Info();
	}

	return info[fileName];
}

// サウンドデータの取得
std::shared_ptr<std::vector<float>> SndLoader::GetData(const std::string & fileName)
{
	if (data.find(fileName) == data.end())
	{
		return nullptr;
	}

	return data[fileName];
}

// 削除
void SndLoader::Delete(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		info.erase(info.find(fileName));
		data.erase(data.find(fileName));
	}
}
