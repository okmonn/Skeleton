#include "SndFunc.h"
#include "../Helper/Helper.h"
#include <algorithm>

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f

// 円周率
#define PI 3.14159265f

// RIFFの読み込み
int snd::LoadRIFF(RIFF & riff, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	fread(&riff, sizeof(RIFF), 1, file);
	if (help::CheckChar("RIFF", riff.id, _countof(riff.id)) == false
		|| help::CheckChar("WAVE", riff.type, _countof(riff.type)) == false)
	{
		fclose(file);
		return -1;
	}

	return 0;
}

// FMTの読み込み
int snd::LoadFMT(FMT & fmt, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	fread(&fmt, sizeof(FMT), 1, file);
	if (help::CheckChar("fmt ", fmt.id, _countof(fmt.id)) == false)
	{
		fclose(file);
		return -1;
	}

	//拡張部分の読み込み
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);
	}

	return 0;
}

// DATAの読み込み
int snd::LoadDATA(DATA & data, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	//ダミー宣言 
	std::string chunkID("1234");
	while (true)
	{
		fread(&chunkID[0], sizeof(unsigned char) * chunkID.size(), 1, file);

		//DATAチャンク発見
		if (chunkID == "data")
		{
			break;
		}
		//その他のチャンク
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
		}
	}

	//DATAの読み込み 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, file);

	return 0;
}

// 8ビット波形の読み込み
void snd::LoadWave8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		return;
	}

	fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
	std::for_each(data.begin(), data.end(), [&](float& i)->void {
		i /= OVERFLLOW_CHAR - 1.0f;
	});

	return;
}

// 16ビット波形の読み込み
void snd::LoadWave16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		return;
	}

	fread(data.data(), sizeof(short) * data.size(), 1, file);
	std::for_each(data.begin(), data.end(), [&](float& i)->void {
		i /= OVERFLLOW_SHORT;
	});

	return;
}
