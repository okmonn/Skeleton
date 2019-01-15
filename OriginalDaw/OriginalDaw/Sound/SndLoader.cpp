#include "SndLoader.h"
#include <Windows.h>
#include <tchar.h>

// スレッド数
#define THREAD_MAX 5

// オフセット
#define OFFSET 10

// コンストラクタ
SndLoader::SndLoader() : 
	flag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);

	Create();
}

// デストラクタ
SndLoader::~SndLoader()
{
	flag = false;
	for (auto& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// 読み込みテーブルの作成
void SndLoader::Create(void)
{
	load[1][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadMono8(tmp, file);
	};

	load[1][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadMono16(tmp, file);
	};

	load[2][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadStereo8(tmp, file);
	};

	load[2][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadStereo16(tmp, file);
	};
}

// 読み込み
int SndLoader::Load(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		OutputDebugString(_T("\n読み込み済み\n"));
		return -1;
	}

	if (fopen_s(&sound[fileName].file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nファイルの参照：失敗\n"));
		return -1;
	}

	snd::RIFF riff{};
	snd::FMT fmt{};
	snd::DATA data{};

	snd::LoadRIFF(riff, sound[fileName].file);
	snd::LoadFMT(fmt, sound[fileName].file);
	snd::LoadDATA(data, sound[fileName].file);

	//数値の格納
	sound[fileName].channel = fmt.channel;
	sound[fileName].sample  = fmt.sample;
	sound[fileName].bit     = fmt.bit;
	sound[fileName].length  = data.size / fmt.byte / fmt.channel;

	// 別スレッドで読み込み
	bool flag = true;
	while (flag)
	{
		for (auto& i : th)
		{
			if (i.joinable() == false)
			{
				sound[fileName].data.resize((size_t)std::ceilf((float)data.size / (float)(fmt.sample / OFFSET) / (float)fmt.channel));
				i = std::thread(&SndLoader::Stream, this, fileName);
				flag = false;
				break;
			}
		}
	}
}

// 非同期読み込み
void SndLoader::Stream(const std::string & fileName)
{
	std::lock_guard<std::mutex>lock(mtx);

	int read = 0;

	while (std::feof(sound[fileName].file) == 0 && flag == true)
	{
		sound[fileName].data[read].resize(sound[fileName].sample / 10);
		load[sound[fileName].channel][sound[fileName].bit](sound[fileName].data[read], sound[fileName].file);
		++read;
	}

	fclose(sound[fileName].file);
}

// 削除
void SndLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
