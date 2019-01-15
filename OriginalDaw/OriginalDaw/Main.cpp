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
	Application app(winSize);
	Application* app2 = new Application(app, 300);

	while (app.CheckMsg() && help::CheckKey('A') == false)
	{
		app.Clear();
		app2->Clear();

		app.Execution();
		app2->Execution();
	}

	delete app2;
	
	return 0;
}