#include "Application/Application.h"

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Application app(400);
	while (app.CheckMag())
	{
		app.Clear();
		app.Execution();
	}

	return 0;
}