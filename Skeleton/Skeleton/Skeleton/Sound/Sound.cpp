#include "Sound.h"
#include "XAudio2.h"
#include "VoiceCallback.h"
#include "SndLoader.h"
#include <ks.h>
#include <ksmedia.h>

// スピーカー設定用配列
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// バッファ数
#define BUFFER 2

// コンストラクタ
Sound::Sound() : 
	voice(nullptr), threadFlag(true)
{
	info = {};
	data.resize(BUFFER);

	back = std::make_unique<VoiceCallback>();
}

// デストラクタ
Sound::~Sound()
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}
	if (voice != nullptr)
	{
		voice->DestroyVoice();
	}
}

// ソースボイスの生成
long Sound::CreateVoice(void)
{
	return 0;
}

// 読み込み
int Sound::Load(const std::string & fileName)
{
	if (SndLoader::Get().Load(fileName) != 0)
	{
		return -1;
	}

	name = fileName;
	info = SndLoader::Get().GetInfo(fileName);

	return 0;
}
