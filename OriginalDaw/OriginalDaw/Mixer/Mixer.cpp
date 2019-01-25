#include "Mixer.h"
#include "../Input/Input.h"
#include "../Application/Application.h"
#include "../Waveform/Waveform.h"
#include "../Characteristic/Characteristic.h"

// スレッド数
#define THREAD_MAX 2

// ウィンドウサイズ
const Vec2 winSize = { 200, 200 };

// コンストラクタ
Mixer::Mixer() : 
	playFlag(false), threadFlag(true)
{
	th.resize(THREAD_MAX);

	Init();
}

// デストラクタ
Mixer::~Mixer()
{
	threadFlag = false;
	for (auto& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// 初期化
void Mixer::Init(void)
{
	app = std::make_shared<Application>(winSize);
}

// 描画
void Mixer::Draw(void)
{
	app->Clear();
	app->Execution();
}

// 処理
void Mixer::UpData(void)
{
	auto drop = app->GetDropFilePath();
	if (drop.find_last_of(".wav") != std::string::npos)
	{
		threadFlag = false;
		for (auto& i : th)
		{
			if (i.joinable() == true)
			{
				i.join();
			}
		}
		sound.reset(new Sound(drop));
		wave.reset(new Waveform(app, sound));
		chara.reset(new Characteristic(app, sound));
		threadFlag = true;
		th[0] = std::thread(&Mixer::DrawWave, this);
		th[1] = std::thread(&Mixer::DrawChara, this);
	}

	static float v = 1.0f;
	if (Input::Get().InputKey(INPUT_DOWN))
	{
		v -= 0.1f;
		if (v < 0.0f)
		{
			v = 0.0f;
		}
	}
	else if (Input::Get().InputKey(INPUT_UP))
	{
		v += 0.1f;
	}
	if (sound != nullptr)
	{
		sound->SetVolume(v);
	}

	if (Input::Get().Triger(INPUT_SPACE))
	{
		if (playFlag == false)
		{
			sound->Play(false);
			playFlag = true;
		}
		else
		{
			sound->Stop();
			playFlag = false;
		}
	}
}

// サウンドの波形周波数の描画
void Mixer::DrawWave(void)
{
	while (threadFlag)
	{
		wave->Draw();
		wave->UpData();
	}
}

// 周波数特性の描画
void Mixer::DrawChara(void)
{
	while (threadFlag)
	{
		chara->Draw();
		chara->UpData();
	}
}

// 処理
void Mixer::Run(void)
{
	while (app->CheckMsg() && Input::Get().InputKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
