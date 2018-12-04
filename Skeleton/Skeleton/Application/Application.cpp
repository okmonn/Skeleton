#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Union/Union.h"
#include "../Effector/Effector.h"
#include "../Sound/Sound.h"
#include <Windows.h>

bool flag = true;

// コンストラクタ
Application::Application()
{
	Create();
}

// デストラクタ
Application::~Application()
{
	flag = false;
	th.join();
}

// クラスの生成
void Application::Create(void)
{
	win      = std::make_shared<Window>();
	input    = std::make_shared<Input>(win);
	un       = std::make_shared<Union>(win);
	effector = std::make_shared<Effector>(un->GetDev(), L"Shader/Effect.hlsl");
	th = std::thread(&Application::Test, this);

	sound = std::make_shared<Sound>(effector);
	sound->Load("animal.wav");
	sound->Play(true);
}

// テスト
void Application::Test(void)
{
	static float freq = 0.0f;
	bool low = false;
	while (flag)
	{
		if (input->Triger(INPUT_SPACE))
		{
			low = (low == true) ? false : true;
			effector->BundPassFilter(low);
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

// メッセージの確認
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//仮想キーメッセージを文字メッセージに変換
		TranslateMessage(&msg);
		//1つのウィドウプロシージャにメッセージを送出する
		DispatchMessage(&msg);
	}

	return true;
}

// キーの入力
bool Application::InputKey(const int & i)
{
	return input->CheckKey(i);
}

// 描画
void Application::Draw(void)
{
	un->Clear();

	//ここに描画・処理

	un->Execution();
}
