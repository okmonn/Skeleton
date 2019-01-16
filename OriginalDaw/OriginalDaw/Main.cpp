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
	Application app(200);

	Sound s("mtgx.wav");
	s.Play(false);

	while (app.CheckMsg() && help::CheckKey(INPUT_ESCAPE) == false)
	{
		app.Clear();
		
		app.Execution();
	}

	return 0;
}