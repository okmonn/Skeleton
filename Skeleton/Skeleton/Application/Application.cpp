#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Union/Union.h"
#include "../Compute/Effector.h"
#include "../Sound/Sound.h"
#include <Windows.h>

int n = 0;

// �R���X�g���N�^
Application::Application()
{
	Create();
}

// �f�X�g���N�^
Application::~Application()
{
	int n = 0;
}

// �N���X�̐���
void Application::Create(void)
{
	win      = std::make_shared<Window>();
	input    = std::make_shared<Input>(win);
	un       = std::make_shared<Union>(win);
	effe     = std::make_shared<Effector>(un->GetDev(), L"Shader/SoundEffect.hlsl");

	sound = std::make_shared<Sound>(effe);
	sound->Load("�u�ԃZ���`�����^��.mp3");
	sound->Play(true);

	un->LoadPmd("model/�������J.pmd", n);
	un->Attach("���S�R���_���X.vmd", n);
}

// �e�X�g
void Application::Test(void)
{
	float pan = 0.0f;
	if (input->CheckKey(INPUT_UP))
	{
		
	}
	else if (input->CheckKey(INPUT_DOWN))
	{
		
	}
	else if (input->CheckKey(INPUT_RIGHT))
	{
		++pan;
		effe->SetPan(1.0f + pan, 1.0f - pan);
	}
	else if (input->CheckKey(INPUT_LEFT))
	{
		--pan;
		effe->SetPan(1.0f + pan, 1.0f - pan);
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
	Test();

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
