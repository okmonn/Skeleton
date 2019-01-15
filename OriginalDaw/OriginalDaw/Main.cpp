#include "Application/Application.h"
#include <Windows.h>

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	auto winSize = help::GetDisplayResolution();
	Application app(200);

	int a = 0;
	Sound s("mtgx.wav");

	while (app.CheckMsg() && help::CheckKey(INPUT_ESCAPE) == false)
	{
		app.Clear();
		if (help::CheckKey(INPUT_SPACE))
		{
			s.Play(false);
		}
		app.Execution();
	}

	return 0;
}