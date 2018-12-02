#pragma once
#include <string>
#include <vector>

struct _iobuf;
typedef _iobuf FILE;

namespace sound
{
	// RIFF
	struct RIFF {
		unsigned char id[4];
		long size;
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		unsigned char id[4];
		long size;
		short type;
		short channel;
		long sample;
		long byte;
		short block;
		short bit;
	};

	// DATA
	struct DATA {
		std::string id;
		long size;
	};

	// 文字列の確認
	bool CheckChar(const unsigned char* data, const std::string& find, const unsigned int& size);

	// RIFFの読み込み
	int LoadRIFF(sound::RIFF& riff, FILE* file);

	// FMTの読み込み
	int LoadFMT(sound::FMT& fmt, FILE* file);

	// DATAの読み込み
	int LoadDATA(sound::DATA& data, FILE* file);

	// モノラル・8ビット
	void LoadMono8(std::vector<float>& data, FILE* file);

	// モノラル・16ビット
	void LoadMono16(std::vector<float>& data, FILE* file);

	// ステレオ・8ビット
	void LoadStereo8(std::vector<float>& data, FILE* file);

	// ステレオ・16ビット
	void LoadStereo16(std::vector<float>& data, FILE* file);
};
