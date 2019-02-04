#include "Mixer.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"
#include "../Manager/DistortionMane.h"
#include "../Manager/FilterMane.h"
#include "../Wave/Wave.h"
#include "../Characteristic/Characteristic.h"
#include <algorithm>

// �E�B���h�E�T�C�Y
const Vec2 WinSize = { 400, 400 };

// �X���b�h��
#define THREAD_NUM 2

// �R���X�g���N�^
Mixer::Mixer() : 
	play(false), threadFlag(true)
{
	app = std::make_shared<Application>(WinSize,Vec2(use::GetDisplayResolution().x / 2 - WinSize.x / 2, use::GetDisplayResolution().y / 2 - WinSize.y / 2));
	app->ChangeTitle("�~�L�T�[");
	mouse = std::make_shared<Mouse>(app);

	th.resize(THREAD_NUM);
}

// �f�X�g���N�^
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

// �`��
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

// �Đ��Ǘ�
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

// ����
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

// �g�`�̕`��
void Mixer::DrawWave(void)
{
	while (threadFlag)
	{
		wave->Draw();
	}
}

// �����̕`��
void Mixer::DrawChara(void)
{
	while (threadFlag)
	{
		chara->Draw();
	}
}

// ���s
void Mixer::Execution(void)
{
	while (app->CheckMag() && Input::Get().CheckKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
}
