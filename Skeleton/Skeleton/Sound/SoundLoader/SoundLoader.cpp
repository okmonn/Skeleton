#include "SoundLoader.h"
#include "../SoundFunc/SoundFunc.h"
#include <mutex>

// 読み込みスレッド最大数
#define THREAD_MAX 5

// コンストラクタ
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	th.resize(THREAD_MAX);

	InitLoadTbl();
}

// デストラクタ
SoundLoader::~SoundLoader()
{
	threadFlag = false;
	for (auto& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// 読み込みテーブルの生成
void SoundLoader::InitLoadTbl(void)
{
	load[1][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadMono8(tmp, file);
	};

	load[1][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadMono16(tmp, file);
	};

	load[2][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadStereo8(tmp, file);
	};

	load[2][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadStereo16(tmp, file);
	};
}

// 読み込み
int SoundLoader::Load(const std::string & fileName)
{
	if (wave.find(fileName) != wave.end())
	{
		return 0;
	}

	if (fopen_s(&wave[fileName].file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	sound::RIFF riff{};
	sound::FMT fmt{};
	sound::DATA data{};

	sound::LoadRIFF(riff, wave[fileName].file);
	sound::LoadFMT(fmt, wave[fileName].file);
	sound::LoadDATA(data, wave[fileName].file);

	wave[fileName].channel = fmt.channel;
	wave[fileName].sample  = fmt.sample;
	wave[fileName].bit     = fmt.bit;

	//別スレッドで読み込み
	bool flag = true;
	while (flag)
	{
		for (auto itr = th.begin(); itr != th.end(); ++itr)
		{
			if (itr->joinable() == false)
			{
				wave[fileName].data.resize((size_t)std::ceilf((float)data.size / (float)(fmt.sample / 10) / (float)fmt.channel));
				*itr = std::thread(&SoundLoader::Stream, this, fileName);
				flag = false;
				break;
			}
		}
	}

	return 0;
}

// 非同期読み込み
void SoundLoader::Stream(const std::string & fileName)
{
	std::mutex m;
	std::lock_guard<std::mutex>lock(m);

	int read = 0;

	while (std::feof(wave[fileName].file) == 0 && threadFlag == true)
	{
		wave[fileName].data[read].resize(wave[fileName].sample * 60);
		load[wave[fileName].channel][wave[fileName].bit](wave[fileName].data[read], wave[fileName].file);
		++read;
	}

	fclose(wave[fileName].file);
}
