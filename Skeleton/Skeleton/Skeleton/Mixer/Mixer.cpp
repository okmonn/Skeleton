#include "Mixer.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"
#include "../Manager/DistortionMane.h"
#include "../Manager/FilterMane.h"
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
	app = std::make_shared<Application>(WinSize,Vec2(use::GetDisplayResolution().x / 2 - WinSize.x / 2, use::GetDisplayResolution().y / 2 - WinSize.y / 2));
	app->ChangeTitle("ミキサー");
	mouse = std::make_shared<Mouse>(app);

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
	if (sound != nullptr)
	{
		distortion->Draw();
		filter->Draw();
	}
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
	mouse->UpData();

	auto pass = app->GetDropFilePass();
	static snd::DelayParam delay = { 0.5f, 0.0f, 0 };
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
		distortion.reset(new DistortionMane(app, sound, mouse));
		filter.reset(new FilterMane(app, sound, mouse));
		wave.reset(new Wave(app, sound));
		chara.reset(new Characteristic(app, sound));
		
		th[0] = std::thread(&Mixer::DrawWave, this);
		th[1] = std::thread(&Mixer::DrawChara, this);
		play  = false;
		return;
	}

	Play();

	if (sound == nullptr)
	{
		return;
	}
	if (Input::Get().CheckKey(INPUT_RIGHT))
	{
		delay.delayTime += 0.05f;
		if (delay.delayTime > 1.0f)
		{
			delay.delayTime = 1.0f;
		}
		sound->SetDelay(delay);
	}
	else if (Input::Get().CheckKey(INPUT_LEFT))
	{
		delay.delayTime -= 0.05f;
		if (delay.delayTime < 0.0f)
		{
			delay.delayTime = 0.0f;
		}
		sound->SetDelay(delay);
	}
	else if (Input::Get().CheckKey(INPUT_W))
	{
		delay.loop += 1;
		if (delay.loop > 10)
		{
			delay.loop = 10;
		}
		sound->SetDelay(delay);
	}
	else if (Input::Get().CheckKey(INPUT_S))
	{
		delay.loop = ((int)delay.loop - 1 < 0) ? 0 : delay.loop - 1;
		sound->SetDelay(delay);
	}

	distortion->UpData();
	filter->UpData();
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
