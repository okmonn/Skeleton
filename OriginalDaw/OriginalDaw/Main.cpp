#include "Application/Application.h"
#include "Input/Input.h"
#include <Windows.h>
#include <vector>

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

	int n = 0;
	app.LoadTex(n, "handle.png");
	float angle = 0.0f;

	while (app.CheckMsg() && help::CheckKey(INPUT_ESCAPE) == false)
	{
		app.Clear();
		app.DrawTex(n, 0.0f, 100.0f, 0.0f, 225.0f, 1.0f, help::Radian(angle));
		app.Execution();

		if (help::CheckKey(INPUT_RIGHT))
		{
			++angle;
		}

		if (i.Triger(INPUT_SPACE))
		{
			s.Play(false);
		}
		else if (i.Triger(INPUT_A))
		{
			s.Stop();
		}
		else if (i.Triger(INPUT_RETURN))
		{
			s.SetLoopPos();
		}
		else if (i.Triger(INPUT_Z))
		{
			s.MoveLoopPos();
		}
	}

	return 0;
}