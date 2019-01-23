#include "Mixer.h"
#include "../Input/Input.h"
#include "../Application/Application.h"
#include "../Waveform/Waveform.h"

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
		threadFlag = true;
		th[0] = std::thread(&Mixer::DrawWave, this);
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

// 処理
void Mixer::Run(void)
{
	while (app->CheckMsg() && Input::Get().InputKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
