#include "Mixer.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"
#include "../Manager/DistortionMane.h"
#include "../Manager/NoiseMane.h"
#include "../Manager/CompMane.h"
#include "../Manager/VolumeMane.h"
#include "../Manager/DelayMane.h"
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
		noise->Draw();
		comp->Draw();
		vol->Draw();
		delay->Draw();
		filter->Draw();
	}
	app->Execution();
}

// 再生管理
void Mixer::Play(void)
{
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
		noise.reset(new NoiseMane(app, sound, mouse));
		comp.reset(new CompMane(app, sound, mouse));
		vol.reset(new VolumeMane(app, sound, mouse));
		delay.reset(new DelayMane(app, sound, mouse));
		filter.reset(new FilterMane(app, sound, mouse));
		wave.reset(new Wave(app, sound));
		chara.reset(new Characteristic(app, sound));
		
		th[0] = std::thread(&Mixer::DrawWave, this);
		th[1] = std::thread(&Mixer::DrawChara, this);
		play  = false;
		return;
	}

	if (sound == nullptr)
	{
		return;
	}

	Play();

	distortion->UpData();
	noise->UpData();
	comp->UpData();
	vol->UpData();
	delay->UpData();
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
