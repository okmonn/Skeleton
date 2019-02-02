#pragma once
#include <string>
#include <vector>

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

	// サウンド情報
	struct Info {
		//チャンネル
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
	};

	// パラメータ
	struct Param {
		//音量
		float volume;
		//増幅率
		float amp;
		//音量レベル
		float level;
		//閾値
		float threshold;
		//圧縮比率
		float ratio;
	};

	// ディレイパラメータ
	struct DelayParam {
		//音量減衰率
		float attenuation;
		//遅延時間
		float delayTime;
		//ループ回数
		unsigned int loop;
	};

	// RIFFの読み込み
	int LoadRIFF(RIFF& riff, FILE* file);

	// FMTの読み込み
	int LoadFMT(FMT& fmt, FILE* file);

	// DATAの読み込み
	int LoadDATA(DATA& data, FILE* file);

	// 8ビット波形の読み込み
	int LoadWave8(std::vector<float>& data, FILE* file);

	// 16ビット波形の読み込み
	int LoadWave16(std::vector<float>& data, FILE* file);
}
