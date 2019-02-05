#include "Sound.h"
#include "XAudio2.h"
#include "VoiceCallback.h"
#include "SndLoader.h"
#include "../Compute/Effector.h"
#include "Delay.h"
#include "Filter.h"
#include "../Useful/Useful.h"
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>

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

// オフセット
#define OFFSET 100

// コンストラクタ
Sound::Sound() : 
	voice(nullptr), index(0), read(0), loopPos(0), loop(false), threadFlag(true)
{
	info  = {};
	data.resize(BUFFER);

	back    = std::make_unique<VoiceCallback>();
	effe    = std::make_unique<Effector>("Shader/Effector.hlsl");
	delay   = std::make_unique<Delay>();
	filter  = std::make_unique<Filter>();
	st      = std::make_unique<XAUDIO2_VOICE_STATE>();
}

// コンストラクタ
Sound::Sound(const std::string & fileName) :
	voice(nullptr), index(0), read(0), loopPos(0), loop(false), threadFlag(true)
{
	info = {};
	data.resize(BUFFER);

	back   = std::make_unique<VoiceCallback>();
	effe   = std::make_unique<Effector>("Shader/Effector.hlsl");
	delay  = std::make_unique<Delay>();
	filter = std::make_unique<Filter>();
	st     = std::make_unique<XAUDIO2_VOICE_STATE>();

	Load(fileName);
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

	auto hr = XAudio2::Get().GetAudio()->CreateSourceVoice(&voice, reinterpret_cast<WAVEFORMATEX*>(&desc), 0, 1.0f, &(*back));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
	}

	return hr;
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

	CreateVoice();

	effe->Init(info.sample * use::Byte(info.bit) * info.channel / OFFSET);

	if (th.joinable() == false)
	{
		th = std::thread(&Sound::Stream, this);
	}

	return 0;
}

// 再生
long Sound::Play(const bool & loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nサウンドの再生：失敗\n"));
		return hr;
	}

	this->loop = loop;

	return hr;
}

// 停止
long Sound::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nサウンドの停止：失敗\n"));
	}

	return hr;
}

// ループ位置のセット
void Sound::SetLoopPos(void)
{
	loopPos = read;
}

// ループ位置に移動
void Sound::MoveLoopPos(void)
{
	WaitForSingleObject(back->handle, INFINITE);

	st->BuffersQueued = 0;
	read = loopPos;
}

// パラメータの取得
snd::Param Sound::GetParam(void) const
{
	return effe->GetParam();
}

// パラメータのセット
void Sound::SetParam(const snd::Param & param)
{
	effe->Copy(param);
}

// ディレイパラメータの取得
snd::DelayParam Sound::GetDelay(void) const
{
	return delay->GetParam();
}

// ディレイパラメータのセット
void Sound::SetDelay(const snd::DelayParam & param)
{
	delay->SetParam(param);
}

// ローパス
void Sound::LowPass(const float & cut, const float & q)
{
	filter->LowPass(cut, q, info);
}

// ハイパス
void Sound::HighPass(const float & cut, const float & q)
{
	filter->HighPass(cut, q, info);
}

// バンドパス
void Sound::BandPass(const float & cut, const float & bw)
{
	filter->BandPass(cut, bw, info);
}

// 非同期処理
void Sound::Stream(void)
{
	const unsigned int bps = info.sample * use::Byte(info.bit) * info.channel / OFFSET;
	unsigned int size = 0;
	XAUDIO2_BUFFER buf{};
	while (threadFlag)
	{
		voice->GetState(&(*st));
		if (st->BuffersQueued >= BUFFER)
		{
			continue;
		}

		size = (SndLoader::Get().GetData(name)->size() - read > bps)
			? bps
			: SndLoader::Get().GetData(name)->size() - read - 1;

		std::vector<float>tmp(&SndLoader::Get().GetData(name)->at(read), &SndLoader::Get().GetData(name)->at(read + size));
		tmp = effe->Execution(tmp);
		tmp = delay->Execution(tmp, info, read);
		tmp = filter->Execution(tmp);
		data[index] = tmp;

		buf.AudioBytes = static_cast<unsigned int>(sizeof(float) * data[index].size());
		buf.pAudioData = reinterpret_cast<unsigned char*>(data[index].data());
		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nサウンドデータの追加：失敗\n"));
			continue;
		}

		index = (index + 1 >= BUFFER) ? 0 : ++index;
		read += size;
		if (read + 1 >= SndLoader::Get().GetData(name)->size())
		{
			if (loop == false)
			{
				Stop();
				index = 0;
				for (auto& i : data)
				{
					memset(i.data(), 0, sizeof(float) * i.size());
				}
			}
			read = 0;
			delay->Clear();
		}
	}
}
