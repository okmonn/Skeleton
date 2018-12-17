#include "Sound.h"
#include "XAudio2/XAudio2.h"
#include "XAudio2/VoiceCallback.h"
#include "SoundLoader/SoundLoader.h"
#include "../Compute/Effector.h"
#include "Filter/Filter.h"
#include "Destroy.h"
#include <ks.h>
#include <ksmedia.h>

// バッファの最大数
#define BUF_MAX 3

// スレッド開始の待機時間
#define WAIT_TIME 5

// スピーカー設定用配列
const DWORD spk[] = {
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
	audio(XAudio2::Get()), loader(SoundLoader::Get()),
	voice(nullptr), loop(false), end(false), threadFlag(true), read(0)
{
	wave.resize(BUF_MAX);

	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
}

// コンストラクタ
Sound::Sound(std::weak_ptr<Effector> effe) : 
	effe(effe), audio(XAudio2::Get()), loader(SoundLoader::Get()),
	voice(nullptr), loop(false), end(false), threadFlag(true), read(0)
{
	wave.resize(BUF_MAX);

	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
}

// デストラクタ
Sound::~Sound()
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}

	Destroy(voice);
}

// ソースボイスの生成
long Sound::CreateVoice(const std::string& fileName)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = loader.GetChannel(fileName);
	desc.Format.nSamplesPerSec  = loader.GetSample(fileName);
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask               = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = audio.GetAudio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, &(*call));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
	}

	return hr;
}

// 読み込み
void Sound::Load(const std::string & fileName)
{
	loader.Load(fileName);

	CreateVoice(fileName);

	name = fileName;

	if (!effe.expired())
	{
		effe.lock()->Init(loader.GetSample(fileName) / 10);
	}

	if (th.joinable() == false)
	{
		th = std::thread(&Sound::Stream, this);
	}
}

// 非同期処理
void Sound::Stream(void)
{
	XAUDIO2_VOICE_STATE st{};

	Sleep(WAIT_TIME);

	unsigned int index = 0;
	while (threadFlag)
	{
		voice->GetState(&st);
		if (st.BuffersQueued >= BUF_MAX)
		{
			continue;
		}

		if (effe.expired())
		{
			wave[index] = loader.GetWave(name)[read];
		}
		else
		{
			effe.lock()->Execution(loader.GetWave(name)[read], wave[index]);
		}

		filter->Execution(wave[index]);

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * static_cast<unsigned int>(wave[index].size());
		buf.pAudioData = (unsigned char*)wave[index].data();

		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nバッファの追加：失敗\n"));
			continue;
		}

		if (read + 1 >= loader.GetWave(name).size())
		{
			if (loop == false)
			{
				Stop();
				end = true;
			}
			read  = 0;
			index = 0;
		}
		else
		{
			++read;
			index = (index + 1 >= BUF_MAX) ? 0 : ++index;
		}
	}
}

// 再生
long Sound::Play(const bool & loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n再生：失敗\n"));
		return hr;
	}

	end        = false;
	this->loop = loop;

	return hr;
}

// 停止
long Sound::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n停止：失敗\n"));
	}

	return hr;
}

// コールバックハンドルの取得
void * Sound::GetHandle(void) const
{
	return call->handle;
}

