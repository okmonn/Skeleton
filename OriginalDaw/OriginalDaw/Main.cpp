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

		Application::Get().DrawLine(0.0f, 200.0f, { 1.0f, 0.0f, 0.0f, 1.0f });

		Application::Get().Execution();
	}

	return 0;
}