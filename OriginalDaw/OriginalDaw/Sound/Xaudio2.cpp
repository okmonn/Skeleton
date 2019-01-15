#include "Xaudio2.h"
#include <xaudio2.h>
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
Xaudio2::Xaudio2() : 
	audio(nullptr)
{
	InitCom();
	Create();
}

// デストラクタ
Xaudio2::~Xaudio2()
{
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
