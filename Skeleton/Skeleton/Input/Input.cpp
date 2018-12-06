// バージョン
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Input.h"
#include "../Window/Window.h"
#include "../etc/Release.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// コンストラクタ
Input::Input(std::weak_ptr<Window>win) :
	win(win), input(nullptr), dev(nullptr)
{
	memset(&keys[0], 0, sizeof(keys));
	memset(&olds[0], 0, sizeof(olds));

	Init();
}

// デストラクタ
Input::~Input()
{
	Release(dev);
	Release(input);
}

// インプットの生成
long Input::CreateInput(void)
{
	auto hr = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&input), nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nインプットの生成：失敗\n"));
	}

	return hr;
}

// キーデバイスの生成
long Input::CreateDev(void)
{
	auto hr = input->CreateDevice(GUID_SysKeyboard, &dev, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nキーデバイスの生成：失敗\n"));
	}

	return hr;
}

// キーフォーマットのセット
long Input::SetKeyFormat(void)
{
	auto hr = dev->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nキーフォーマットのセット：失敗\n"));
	}

	return hr;
}

// キーの協調レベルのセット
long Input::SetKeyCooperative(void)
{
	auto hr = dev->SetCooperativeLevel(reinterpret_cast<HWND>(win.lock()->Get()), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nキーの協調レベルのセット：失敗\n"));
		return hr;
	}

	//入力デバイスへのアクセス権利を取得
	dev->Acquire();

	return hr;
}

// 初期化
void Input::Init(void)
{
	CreateInput();
	CreateDev();
	SetKeyFormat();
	SetKeyCooperative();
}

// キー入力
bool Input::CheckKey(const int & i)
{
	bool flag = false;

	//キー情報を取得
	dev->GetDeviceState(sizeof(keys), &keys);

	if (keys[i] & 0x80)
	{
		flag = true;
	}

	olds[i] = keys[i];

	return flag;
}

// キートリガー
bool Input::Triger(const int & i)
{
	bool flag = false;

	//キー情報を取得
	dev->GetDeviceState(sizeof(keys), &keys);

	if ((keys[i] & 0x80) && !(olds[i] & 0x80))
	{
		flag = true;
	}

	olds[i] = keys[i];

	return flag;
}
