#include "Application/Application.h"

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Application app(400);
	int image = 0;
	app.LoadTex(image, "avicii.png");
	while (app.CheckMag())
	{
		app.Clear();
		app.DrawTex(image, 0.0f, 200.0f, 0.0f, 320.0f);
		app.Execution();
	}

	return 0;
}