#include "Xaudio2.h"
#include <xaudio2.h>
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
Xaudio2::Xaudio2() : 
	audio(nullptr), mastering(nullptr)
{
	InitCom();
	Create();
	CreateMastering();
}

// デストラクタ
Xaudio2::~Xaudio2()
{
	if (mastering != nullptr)
	{
		mastering->DestroyVoice();
	}
	if (audio != nullptr)
	{
		audio->Release();
	}
	CoUninitialize();
}

// COMの初期化
long Xaudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOMの初期化：失敗\n"));
	}

	return hr;
}

// Xaudio2の生成
long Xaudio2::Create(void)
{
	auto hr = XAudio2Create(&audio);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nXaudio2の生成：失敗\n"));
	}

	return hr;
}

// マスタリングの生成
long Xaudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマスタリングの生成：失敗\n"));
	}

	return hr;
}
