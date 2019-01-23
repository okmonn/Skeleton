#include "Mixer.h"
#include "../Input/Input.h"
#include "../Application/Application.h"

// �X���b�h��
#define THREAD_MAX 2

// �R���X�g���N�^
Mixer::Mixer() : 
	playFlag(false), threadFlag(true)
{
	Init();
}

// �f�X�g���N�^
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

// ������
void Mixer::Init(void)
{
	app = std::make_shared<Application>(200);
}

// �`��
void Mixer::Draw(void)
{
	app->Clear();
	app->Execution();
}

// ����
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

// ����
void Mixer::Run(void)
{
	while (app->CheckMsg() && Input::Get().InputKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
