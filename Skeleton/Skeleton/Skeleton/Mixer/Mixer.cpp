#include "Mixer.h"
#include "../Application/Application.h"
#include "../Wave/Wave.h"

// ウィンドウサイズ
const Vec2 WinSize = { 400, 400 };

// コンストラクタ
Mixer::Mixer() : 
	play(false), threadFlag(true)
{
	app = std::make_shared<Application>(WinSize);
}

// デストラクタ
Mixer::~Mixer()
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}
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
		sound.reset(new Sound(pass));
		wave.reset(new Wave(app, sound));
		threadFlag = false;
		if (th.joinable() == true)
		{
			th.join();
		}
		threadFlag = true;
		th = std::thread(&Mixer::DrawWave, this);
		play = false;
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

// 実行
void Mixer::Execution(void)
{
	while (app->CheckMag() && Input::Get().CheckKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
