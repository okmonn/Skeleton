#include "Application/Application.h"
#include "Input/Input.h"
#include <Windows.h>

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	auto& i = Input::Get();
	auto winSize = help::GetDisplayResolution();
	Application app(winSize);

	Sound s("mtgx.wav");
	s.Play(false);

	int n = 0;
	app.LoadTex(n, "avicii.png");

	while (app.CheckMsg() && help::CheckKey(INPUT_ESCAPE) == false)
	{
		app.Clear();
		app.DrawTex(n, 0.0f, 100.0f, 0.0f, 320.0f);
		app.Execution();
	}

	return 0;
}