#include "SndLoader.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ
SndLoader::SndLoader()
{
	sound.clear();

	Create();
}

// デストラクタ
SndLoader::~SndLoader()
{
}

// 読み込みテーブルの作成
void SndLoader::Create(void)
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

	sound[fileName].data.resize(data.size);

	load[sound[fileName].bit](sound[fileName].data, sound[fileName].file);

	return 0;
}

// 削除
void SndLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
