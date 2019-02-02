#include "Mixer.h"
#include "../Application/Application.h"
#include "../Wave/Wave.h"
#include "../Characteristic/Characteristic.h"
#include <algorithm>

// ウィンドウサイズ
const Vec2 WinSize = { 400, 400 };

// スレッド数
#define THREAD_NUM 2

// コンストラクタ
Mixer::Mixer() : 
	play(false), threadFlag(true)
{
	app = std::make_shared<Application>(WinSize);

	th.resize(THREAD_NUM);
}

// デストラクタ
Mixer::~Mixer()
{
	threadFlag = false;
	std::for_each(th.begin(), th.end(), [&](std::thread& th)->void {
		if (th.joinable() == true)
		{
			th.join();
		}
	});
}

// 描画
void Mixer::Draw(void)
{
	app->Clear();
	app->Execution();
}

// 再生管理
void Mixer::Play(void)
{
	if (sound == nullptr)
	{
		return;
	}

	if (Input::Get().CheckTrigger(INPUT_SPACE))
	{
		if (play == false)
		{
			sound->Play(true);
			play = true;
		}
		else
		{
			sound->Stop();
			play = false;
		}
	}
}

// 処理
void Mixer::UpData(void)
{
	auto pass = app->GetDropFilePass();
	if (pass.find_last_of(".wav") != std::string::npos)
	{
		if (sound != nullptr)
		{
			sound->Stop();
		}

		threadFlag = false;
		std::for_each(th.begin(), th.end(), [&](std::thread& th)->void {
			if (th.joinable() == true)
			{
				th.join();
			}
		});
		threadFlag = true;

		sound.reset(new Sound(pass));
		wave.reset(new Wave(app, sound));
		chara.reset(new Characteristic(app, sound));
		
		th[0] = std::thread(&Mixer::DrawWave, this);
		th[1] = std::thread(&Mixer::DrawChara, this);
		play  = false;
		return;
	}

	Play();
}

// 波形の描画
void Mixer::DrawWave(void)
{
	while (threadFlag)
	{
		wave->Draw();
	}
}

// 特性の描画
void Mixer::DrawChara(void)
{
	while (threadFlag)
	{
		chara->Draw();
	}
}

// 実行
void Mixer::Execution(void)
{
	while (app->CheckMag() && Input::Get().CheckKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
