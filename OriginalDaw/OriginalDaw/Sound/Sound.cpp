#include "Sound.h"
#include "Xaudio2.h"
#include "SndLoader.h"
#include "VoiceCallback.h"
#include "Filter.h"
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>

// バッファ数
#define BUFFER 2

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

// コンストラクタ
Sound::Sound() : 
	voice(nullptr), loop(false), flag(true), read(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	sound  = {};
}

// コンストラクタ
Sound::Sound(const std::string & fileName) :
	voice(nullptr), loop(false), flag(true), read(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	sound  = {};

	Load(fileName);
}

// コンストラクタ
Sound::Sound(const snd::Info & info) :
	voice(nullptr), loop(false), flag(true), read(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	sound  = {};

	CopyInfo(info);
}

// デストラクタ
Sound::~Sound()
{
	flag = false;
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
long Sound::CreateVoice(const std::string & fileName)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = SndLoader::Get().GetSnd(fileName).channel;
	desc.Format.nSamplesPerSec  = SndLoader::Get().GetSnd(fileName).sample;
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask               = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = Xaudio2::Get().GetAudio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, &(*call));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
		return hr;
	}

	sound.channel = desc.Format.nChannels;
	sound.sample  = desc.Format.nSamplesPerSec;
	sound.bit     = SndLoader::Get().GetSnd(fileName).bit;
	sound.length  = SndLoader::Get().GetSnd(fileName).length;

	return hr;
}

// ソースボイスの生成
long Sound::CreateVoice(const snd::Info & info)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = info.channel;
	desc.Format.nSamplesPerSec  = info.sample;
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask               = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = Xaudio2::Get().GetAudio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
		return hr;
	}

	sound.channel = desc.Format.nChannels;
	sound.sample  = desc.Format.nSamplesPerSec;
	sound.bit     = info.bit;
	sound.length  = info.data.size() / desc.Format.nAvgBytesPerSec / desc.Format.nChannels;

	return hr;
}

// 読み込み
void Sound::Load(const std::string & fileName)
{
	if (th.joinable() == true)
	{
		return;
	}

	SndLoader::Get().Load(fileName);
	CreateVoice(fileName);

	name = fileName;

	sound.data.resize(BUFFER);

	th = std::thread(&Sound::StreamFile, this);
}

// サウンド情報からサウンドの生成
void Sound::CopyInfo(const snd::Info & info)
{
	CreateVoice(info);

	for (auto& i : sound.data)
	{
		i = info.data;
	}
}

// 再生
long Sound::Play(const bool& loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n再生：失敗\n"));
		return hr;
	}

	this->loop = loop;
	return hr;
}

// 停止
long Sound::Stop(void)
{
	WaitForSingleObject(call->handle, INFINITE);

	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("停止：失敗\n"));
	}

	return hr;
}

// 再生ポイントのリセット
void Sound::Reset(void)
{
	state->BuffersQueued = 0;
	read = 0;
}

// ファイルからの非同期処理
void Sound::StreamFile(void)
{
	unsigned int index = 0;
	while (flag)
	{
		voice->GetState(&(*state));
		if (state->BuffersQueued >= BUFFER)
		{
			continue;
		}

		sound.data[index] = SndLoader::Get().GetSnd(name).data[read];
		filter->Execution(sound.data[index]);

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * static_cast<unsigned int>(sound.data[index].size());
		buf.pAudioData = (unsigned char*)sound.data[index].data();
		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nバッファの追加：失敗\n"));
			continue;
		}

		if (read + 1 >= SndLoader::Get().GetSnd(name).data.size())
		{
			if (loop == false)
			{
				Stop();
			}
			read  = 0;
			index = 0;
		}
		else
		{
			++read;
			index = (index + 1 >= BUFFER) ? 0 : ++index;
		}
	}
}

// サウンド情報からの非同期処理
void Sound::StreamInfo(void)
{
	unsigned int index = 0;
	while (flag)
	{
		voice->GetState(&(*state));
		if (state->BuffersQueued >= BUFFER)
		{
			continue;
		}

		filter->Execution(sound.data[index]);

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * static_cast<unsigned int>(sound.data[index].size());
		buf.pAudioData = (unsigned char*)sound.data[index].data();
		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nバッファの追加：失敗\n"));
			continue;
		}

		if (loop == false)
		{
			Stop();
			Reset();
		}
		else
		{
			index = (index + 1 >= BUFFER) ? 0 : ++index;
		}
	}
}

