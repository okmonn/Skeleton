#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Union/Union.h"
#include "../Effector/Effector.h"
#include "../Sound/Sound.h"
#include <Windows.h>

bool flag = true;

int n = 0;

// �R���X�g���N�^
Application::Application()
{
	Create();
}

// �f�X�g���N�^
Application::~Application()
{
	flag = false;
	th.join();
}

// �N���X�̐���
void Application::Create(void)
{
	win      = std::make_shared<Window>();
	input    = std::make_shared<Input>(win);
	un       = std::make_shared<Union>(win);
	effector = std::make_shared<Effector>(un->GetDev(), L"Shader/Effect.hlsl");
	th = std::thread(&Application::Test, this);

	sound = std::make_shared<Sound>(effector);
	sound->Load("mtgx.wav");
	sound->Play(true);

	un->LoadPmd("model/�������J.pmd", n);
	un->Attach("���S�R���_���X.vmd", n);
}

// �e�X�g
void Application::Test(void)
{
	static float freq = 0.0f;
	bool low = false;
	while (flag)
	{
		if (input->Triger(INPUT_SPACE))
		{
			low = (low == true) ? false : true;
			effector->LowPassFilter(low);
		}

		if (input->CheckKey(INPUT_UP))
		{
			freq += 10.0f;
			if (freq > 44100.0f / 2.0f)
			{
				freq = 44100.0f / 2.0f;
			}
			effector->SetFilterParam(freq);
		}
		else if (input->CheckKey(INPUT_DOWN))
		{
			freq -= 10.0f;
			if (freq < 0.0f)
			{
				freq = 0.0f;
			}
			effector->SetFilterParam(freq);
		}
	}
}

// ���b�Z�[�W�̊m�F
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
		TranslateMessage(&msg);
		//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
		DispatchMessage(&msg);
	}

	return true;
}

// �L�[�̓���
bool Application::InputKey(const int & i)
{
	return input->CheckKey(i);
}

// �`��
void Application::Draw(void)
{
	un->Animation(n, true, 0.5f);

	un->ClearShadow();

	un->DrawPmdShadow(n);

	un->ExecutionShadow();

	un->Clear();
	static float angle = 0.0f;
	static float target = 10.0f;

	if (input->CheckKey(INPUT_D))
	{
		++angle;
		un->RotatePmd(n, angle);
	}
	else if (input->CheckKey(INPUT_A))
	{
		--angle;
		un->RotatePmd(n, angle);
	}
	else if (input->CheckKey(INPUT_W))
	{
		++target;
		un->SetCamera(0, 10, -15, 0, target, 0);
	}
	else if (input->CheckKey(INPUT_S))
	{
		--target;
		un->SetCamera(0, 10, -15, 0, target, 0);
	}

	//�����ɕ`��E����
	un->DrawPlane(-25, 0, -25, 50, 1, 50, 1, 0, 0);
	un->DrawPmd(n);

	un->Execution();
}
