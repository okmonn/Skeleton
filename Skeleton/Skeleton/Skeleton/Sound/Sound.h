#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
class VoiceCallback;

class Sound
{
public:
	// コンストラクタ
	Sound();
	// デストラクタ
	~Sound();

	// 読み込み
	int Load(const std::string& fileName);

private:
	// ソースボイスの生成
	long CreateVoice(void);


	// コールバック
	std::unique_ptr<VoiceCallback>back;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// スレッドフラグ
	bool threadFlag;

	// 読み込みファイル名
	std::string name;

	// サウンド情報
	snd::Info info;

	// サウンドバッファ
	std::vector<std::vector<float>>data;

	// スレッド
	std::thread th;
};
