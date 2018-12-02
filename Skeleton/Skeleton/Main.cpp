#include "Application/Application.h"
// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Application& a = Application::Get();
	while (a.CheckMsg() && a.InputKey(INPUT_ESCAPE) != true)
	{
		a.Draw();
	}

	return 0;
}