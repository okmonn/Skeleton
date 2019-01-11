#include "Application/Application.h"
#include <Windows.h>

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	while (Application::Get().CheckMsg() && Application::Get().CheckKey(VK_ESCAPE) == false)
	{
		Application::Get().Clear();
		Application::Get().Execution();
	}

	return 0;
}