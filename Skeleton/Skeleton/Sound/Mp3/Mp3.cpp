#include "Mp3.h"

// ビットレート
const int bitRate[] = {
	0, 32000, 40000, 48000, 56000, 64000, 80000, 96000, 
	112000, 128000, 160000, 192000, 224000, 256000, 320000
};
// サンプリングレート
const int sampleRate[] = {
	44100, 48000, 32000
};
//チャンネル
#define STEREO 2
#define JOINT_STEREO 2
#define DUAL 6
#define MONAURAL
const int channels[] = {
	STEREO, JOINT_STEREO, DUAL, MONAURAL
};

// ヘッダーの読み込み
long mp3::LoadHeader(ID3v2Header & header, FILE * file)
{
	fread(&header.id[0], sizeof(header.id), 1, file);
	for (size_t i = 1; i <= sizeof(header.ver); ++i)
	{
		char tmp = 0;
		fread(&tmp, sizeof(char), 1, file);
		header.ver |= tmp;
		header.ver = header.ver << 8 * (1 - (i / sizeof(header.ver)));
	}
	fread(&header.flag, sizeof(header.flag), 1, file);

	long size = 0;
	for (size_t i = 1; i <= sizeof(header.size); ++i)
	{
		char tmp = 0;
		fread(&tmp, sizeof(char), 1, file);
		header.size |= tmp;
		header.size = header.size << 8 * (1 - (i / sizeof(header.size)));

		size += tmp * (long)pow(128, sizeof(header.size) - i);
	}

	return size + sizeof(header);
}

// フレームの読み込み
void mp3::LoadFream(std::vector<ID3v2Fream> & freams, FILE * file)
{
	while (true)
	{
		ID3v2Fream fream{};
		fread(&fream.id[0], sizeof(fream.id), 1, file);
		if (fream.id[0] == 0)
		{
			fseek(file, -4L, SEEK_CUR);
			break;
		}
		for (size_t i = 1; i <= sizeof(fream.size); ++i)
		{
			char tmp = 0;
			fread(&tmp, sizeof(char), 1, file);
			fream.size |= tmp;
			fream.size = fream.size << 8 * (1 - (i / sizeof(fream.size)));
		}
		fread(&fream.flag, sizeof(fream.flag), 1, file);

		fream.contents.resize(fream.size);
		fread(&fream.contents[0], sizeof(char) * fream.size, 1, file);

		freams.push_back(fream);
	}
}

// MPEGフレームの読み込み
void mp3::LoadMpeg(std::vector<unsigned char>& data, FILE * file)
{
	unsigned char head[4];
	fread(&head[0], sizeof(head), 1, file);
	int bit = head[2] >> 4;
	int sample = (head[2] & ~0xf0) >> 2;
	int channel = head[3] >> 6;

	unsigned char tmp = 0;
	for (unsigned int i = 0; i <= (144 * bitRate[bit] / sampleRate[sample]) - sizeof(head); ++i)
	{
		fread(&tmp, sizeof(tmp), 1, file);
		data.push_back(tmp);
	}
}

// 読み込み
int mp3::Load(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	ID3v2Header header{};
	auto headerSize = LoadHeader(header, file);
	fseek(file, headerSize, SEEK_SET);

	std::vector<unsigned char>data;
	LoadMpeg(data, file);

	fclose(file);

	return 0;
}
