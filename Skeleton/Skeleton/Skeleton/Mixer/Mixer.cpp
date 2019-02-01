#include "Mixer.h"
#include "../Application/Application.h"

// ウィンドウサイズ
const Vec2 WinSize = { 400, 400 };

// コンストラクタ
Mixer::Mixer()
{
	app = std::make_shared<Application>(WinSize);
}

// デストラクタ
Mixer::~Mixer()
{
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
	static bool play = false;
	auto pass = app->GetDropFilePass();
	if (pass.find_last_of(".wav") != std::string::npos)
	{
		sound.reset(new Sound(pass));
		play = false;
		return;
	}

	if (sound != nullptr)
	{
		if (Input::Get().CheckTrigger(INPUT_SPACE) && play == false)
		{
			sound->Play(false);
			play = true;
		}
		else if (Input::Get().CheckTrigger(INPUT_SPACE) && play == true)
		{
			sound->Stop();
			play = false;
		}

		static float cut = 22000.0f;
		if (Input::Get().CheckKey(INPUT_DOWN))
		{
			cut -= 100.0f;
			if (cut < 0.0f)
			{
				cut = 0.0f;
			}
			sound->LowPass(cut);
		}
		else if (Input::Get().CheckKey(INPUT_UP))
		{
			cut += 100.0f;
			if (cut > 22000.0f)
			{
				cut = 22000.0f;
			}
			sound->LowPass(cut);
		}
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
