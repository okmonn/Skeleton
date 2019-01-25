#include "Sound.h"
#include "Xaudio2.h"
#include "SndLoader.h"
#include "VoiceCallback.h"
#include "Filter.h"
#include "../Compute/Effector.h"
#include "../Helper/Helper.h"
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>

// バッファ数
#define BUFFER 2

// オフセット
#define OFFSET 100

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

// パラメータの初期値
const snd::Param paramInit = {
	1.0f
};

// コンストラクタ
Sound::Sound() : 
	voice(nullptr), loop(false), flag(true), index(0), read(0), loopPos(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	effe   = std::make_unique<Effector>(L"Shader/Effector.hlsl");
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	param  = paramInit;

	copy = {};

	data.resize(BUFFER);
}

// コンストラクタ
Sound::Sound(const std::string & fileName) :
	voice(nullptr), loop(false), flag(true), index(0), read(0), loopPos(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	effe   = std::make_unique<Effector>(L"Shader/Effector.hlsl");
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	param  = paramInit;

	copy = {};

	data.resize(BUFFER);

	Load(fileName);
}

// コンストラクタ
Sound::Sound(const snd::Info & info) :
	voice(nullptr), loop(false), flag(true), index(0), read(0), loopPos(0)
{
	call   = std::make_unique<VoiceCallback>();
	filter = std::make_unique<Filter>();
	effe   = std::make_unique<Effector>(L"Shader/Effector.hlsl");
	state  = std::make_unique<XAUDIO2_VOICE_STATE>();
	param  = paramInit;

	copy = {};

	data.resize(BUFFER);

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
long Sound::CreateVoice(void)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = copy.channel;
	desc.Format.nSamplesPerSec  = copy.sample;
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
	}

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

	name = fileName;
	copy.channel = SndLoader::Get().GetSnd(name).channel;
	copy.sample  = SndLoader::Get().GetSnd(name).sample;
	copy.bit     = SndLoader::Get().GetSnd(name).bit;
	copy.length  = SndLoader::Get().GetSnd(name).length;

	CreateVoice();

	effe->Init(copy.sample * help::Byte(copy.bit) * copy.channel / OFFSET);

	th = std::thread(&Sound::StreamFile, this);
}

// サウンド情報構造体のコピー
void Sound::CopyInfo(const snd::Info & info)
{
	if (th.joinable() == true)
	{
		return;
	}

	copy = info;

	CreateVoice();

	effe->Init(copy.sample * help::Byte(copy.bit) * copy.channel / OFFSET);

	th = std::thread(&Sound::StreamInfo, this);
}

// ローパスフィルタ
void Sound::LowPass(const float & cutoff, const float & q)
{
	filter->LowPass(cutoff, static_cast<float>(copy.sample), q);
}

// ハイパスフィルタ
void Sound::HightPass(const float & cutoff, const float & q)
{
	filter->HighPass(cutoff, static_cast<float>(copy.sample), q);
}

// バンドパスフィルタ
void Sound::BandPass(const float & cutoff, const float & bw)
{
	filter->BandPass(cutoff, static_cast<float>(copy.sample), bw);
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
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("停止：失敗\n"));
	}

	return hr;
}

// ループ開始位置のセット
void Sound::SetLoopPos(void)
{
	loopPos = read;
}

// ループ開始位置に移動
void Sound::MoveLoopPos(void)
{
	WaitForSingleObject(call->handle, INFINITE);

	state->BuffersQueued = 0;
	read = loopPos;
}

// ファイルからの非同期処理
void Sound::StreamFile(void)
{
	unsigned int size  = 0;
	while (flag)
	{
		voice->GetState(&(*state));
		if (state->BuffersQueued >= BUFFER)
		{
			continue;
		}

		unsigned int bps = copy.sample * help::Byte(copy.bit) * copy.channel / OFFSET;

		size = (SndLoader::Get().GetSnd(name).data->size() - read > bps) ? 
			bps : SndLoader::Get().GetSnd(name).data->size() - read - 1;
		std::vector<float>tmp(&SndLoader::Get().GetSnd(name).data->at(read), &SndLoader::Get().GetSnd(name).data->at(read + size));

		effe->Copy("param", param);
		effe->Execution(tmp);
		filter->Execution(tmp);
		data[index] = tmp;

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = static_cast<unsigned int>(sizeof(float) * data[index].size());
		buf.pAudioData = (unsigned char*)(data[index].data());
		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nサウンド情報をバッファに追加：失敗\n"));
			continue;
		}

		index = (index + 1 >= BUFFER) ? 0 : ++index;
		read += size;
		if (read + 1 >= SndLoader::Get().GetSnd(name).data->size())
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
		}
	}
}

// コピーデータから非同期処理
void Sound::StreamInfo(void)
{
	unsigned int size = 0;
	while (flag)
	{
		voice->GetState(&(*state));
		if (state->BuffersQueued >= BUFFER)
		{
			continue;
		}

		unsigned int bps = copy.sample * help::Byte(copy.bit) * copy.channel / OFFSET;

		/*size = (copy.data.size() - read - 1> bps) ?
			bps : copy.data.size() - read - 1;*/
		data[index].assign(&copy.data[read], &copy.data[read + bps]);

		effe->Copy("param", param);
		effe->Execution(data[index]);
		filter->Execution(data[index]);

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = static_cast<unsigned int>(sizeof(float) * data[index].size());
		buf.pAudioData = (unsigned char*)(data[index].data());
		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nサウンド情報をバッファに追加：失敗\n"));
			continue;
		}

		index = (index + 1 >= BUFFER) ? 0 : ++index;
		read += size;
		if (read + 1 >= copy.data.size() / help::Byte(copy.bit))
		{
			if (loop == false)
			{
				Stop();
				index = 0;
			}

			read = 0;
		}
	}
}
