#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Union/Union.h"
#include "../Compute/Effector.h"
#include "../Sound/Sound.h"
#include <Windows.h>

int n = 0;

// コンストラクタ
Application::Application()
{
	Create();
}

// デストラクタ
Application::~Application()
{
	int n = 0;
}

// クラスの生成
void Application::Create(void)
{
	win      = std::make_shared<Window>();
	input    = std::make_shared<Input>(win);
	un       = std::make_shared<Union>(win);
	effe     = std::make_shared<Effector>(un->GetDev(), L"Shader/SoundEffect.hlsl");

	sound = std::make_shared<Sound>(effe);
	sound->Load("瞬間センチメンタル.mp3");
	sound->Play(true);

	un->LoadPmd("model/巡音ルカ.pmd", n);
	un->Attach("ヤゴコロダンス.vmd", n);
}

// テスト
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

	//ここに描画・処理
	un->DrawPlane(-25, 0, -25, 50, 1, 50, 1, 0, 0);
	un->DrawPmd(n);

	un->Execution();
}
