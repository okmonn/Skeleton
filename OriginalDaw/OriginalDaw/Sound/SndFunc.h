#pragma once
#include <string>
#include <vector>
#include <memory>

struct _iobuf;
typedef _iobuf FILE;

namespace snd
{
	// RIFF
	struct RIFF {
		//ID
		unsigned char id[4];
		//サイズ
		long size;
		//タイプ
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		//ID
		unsigned char id[4];
		//サイズ
		long size;
		//タイプ
		short type;
		//チャンネル
		short channel;
		//サンプリング周波数
		long sample;
		//バイト
		long byte;
		//ブロックサイズ
		short block;
		//量子化ビット数
		short bit;
	};

	// DATA
	struct DATA {
		//ID
		std::string id;
		//サイズ
		long size;
	};

	//サウンド情報
	struct Snd {
		//ファイルハンドル
		FILE* file;
		//チャンネル数
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
		//再生時間
		int length;
		//波形情報
		std::shared_ptr<std::vector<float>>data;
	};

	// パラメータ
	struct Param {
		//音量
		float volume;
	};

	// サウンド情報
	struct Info {
		// チャンネル数
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
		//再生時間
		int length;
		//波形情報
		std::vector<float>data;
	};

	// RIFFの読み込み
	int LoadRIFF(RIFF& riff, FILE* file);

	// FMTの読み込み
	int LoadFMT(FMT& fmt, FILE* file);

	// DATAの読み込み
	int LoadDATA(DATA& data, FILE* file);

	// 8ビット波形の読み込み
	void LoadWave8(std::vector<float>& data, FILE* file);

	// 16ビット波形の読み込み
	void LoadWave16(std::vector<float>& data, FILE* file);
}
