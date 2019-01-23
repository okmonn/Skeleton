#include "Mixer.h"
#include "../Input/Input.h"
#include "../Application/Application.h"

// スレッド数
#define THREAD_MAX 2

// コンストラクタ
Mixer::Mixer() : 
	playFlag(false), threadFlag(true)
{
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
	app = std::make_shared<Application>(200);
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
		sound.reset(new Sound(drop));
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

// 処理
void Mixer::Run(void)
{
	while (app->CheckMsg() && Input::Get().InputKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
