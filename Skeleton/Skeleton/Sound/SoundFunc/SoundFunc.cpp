#include "SoundFunc.h"

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f

// 円周率
#define PI 3.14159265f

// ステレオ8ビット
struct Stereo8 {
	unsigned char left;
	unsigned char right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// ステレオ16ビット
struct Stereo16 {
	short left;
	short right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// 文字列の確認
bool sound::CheckChar(const unsigned char * data, const std::string & find, const unsigned int & size)
{
	unsigned int index = 0;
	for (unsigned int i = 0; i < size; ++i)
	{
		if (data[i] == find[index])
		{
			if (index + 1 >= find.size())
			{
				return true;
			}

			++index;
		}
	}

	return false;
}

// RIFFの読み込み
int sound::LoadRIFF(sound::RIFF & riff, FILE * file)
{
	fread(&riff, sizeof(RIFF), 1, file);
	if (CheckChar(riff.id, "RIFF", _countof(riff.id)) == false
		|| CheckChar(riff.type, "WAVE", _countof(riff.type)) == false)
	{
		fclose(file);
		return -1;
	}

	return 0;
}

// FMTの読み込み
int sound::LoadFMT(sound::FMT & fmt, FILE * file)
{
	fread(&fmt, sizeof(FMT), 1, file);
	if (CheckChar(fmt.id, "fmt ",_countof(fmt.id)) == false)
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
int sound::LoadDATA(sound::DATA & data, FILE * file)
{
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

// モノラル・8ビット
void sound::LoadMono8(std::vector<float>& data, FILE * file)
{
	unsigned char tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(unsigned char), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを-1～1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR - 1.0f;
	}
}

// モノラル・16ビット
void sound::LoadMono16(std::vector<float>& data, FILE * file)
{
	short tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(short), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを-1～1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}
}

// ステレオ・8ビット
void sound::LoadStereo8(std::vector<float>& data, FILE * file)
{
	Stereo8 tmp{};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo8), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを-1～1の範囲に正規化
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_CHAR - 1.0f;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR - 1.0f;
	}
}

// ステレオ・16ビット
void sound::LoadStereo16(std::vector<float>& data, FILE * file)
{
	Stereo16 tmp{};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo16), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}
}

// ハニング窓関数
float sound::Haninng(const unsigned int & i, const size_t & size)
{
	float tmp = 0.0f;

	tmp = (size % 2 == 0) ?
		//偶数
		0.5f - 0.5f * cosf(2.0f * PI * i / size) :
		//奇数
		0.5f - 0.5f * cosf(2.0f * PI * (i + 0.5f) / size);

	if (tmp == 0.0f)
	{
		tmp = 1.0f;
	}

	return tmp;
}

// シンク関数
float sound::Sinc(const float & i)
{
	return (i == 0.0f) ? 1.0f : sinf(i) / i;
}

// 離散フーリエ変換
void sound::DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real = input;
	imag.assign(input.size(), 0.0f);

	float tmpR = 0.0f;
	float tmpI = 0.0f;
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			tmpR =  cosf(2.0f * PI * i * n / input.size());
			tmpI = -sinf(2.0f * PI * i * n / input.size());

			real[i] += tmpR * (input[n] * Haninng(n, input.size())) - tmpI * 0.0f;
			imag[i] += tmpR * 0.0f                                  + tmpI * (input[n] * Haninng(n, input.size()));
		}
	}
}

// 逆離散フーリエ変換
void sound::IDFT(const std::vector<float>& real, const std::vector<float>& imag, std::vector<float>& out)
{
	out.assign(real.size(), 0.0f);

	float tmpR = 0.0f;
	float tmpI = 0.0f;
	for (unsigned int i = 0; i < real.size(); ++i)
	{
		float tmp = 0.0f;
		for (unsigned int n = 0; n < real.size(); ++i)
		{
			tmpR = cosf(2.0f * PI * i * n / real.size());
			tmpI = sinf(2.0f * PI * i * n / real.size());

			out[i] += (tmpR * real[n] - tmpI * imag[n]) / real.size();
			tmp    += (tmpR * imag[n] + tmpI * real[n]) / real.size();
		}

		out[i] /= Haninng(i, real.size());
		tmp    /= Haninng(i, real.size());
	}
}
