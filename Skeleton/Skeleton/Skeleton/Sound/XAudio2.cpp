#include "XAudio2.h"
#include <xaudio2.h>
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// �R���X�g���N�^
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// �f�X�g���N�^
XAudio2::~XAudio2()
{
	if (audio != nullptr)
	{
		audio->Release();
	}
	if (mastering != nullptr)
	{
		mastering->DestroyVoice();
	}
	CoUninitialize();
}

// COM�̏�����
long XAudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOM�̏������F���s\n"));
	}

	return hr;
}

// �I�[�f�B�I�̐���
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�I�[�f�B�I�̐����F���s\n"));
	}

	return hr;
}

// �}�X�^�����O�̐���
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�}�X�^�����O�̐����F���s\n"));
	}

	return hr;
}

// ������
void XAudio2::Init(void)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}