#include "XAudio2.h"
#include "../Destroy.h"

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// デストラクタ
XAudio2::~XAudio2()
{
	Destroy(mastering);
	Release(audio);

	CoUninitialize();
}

// COMの初期化
long XAudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOMの初期化：失敗\n"));
	}

	return hr;
}

// オーディオの生成
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nオーディオの生成：失敗\n"));
	}

	return hr;
}

// マスタリングの生成
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマスタリングの生成：失敗\n"));
	}

	return hr;
}

// 初期化
void XAudio2::Init(void)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}
