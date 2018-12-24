#include "Mp3.h"

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

	fclose(file);

	return 0;
}
