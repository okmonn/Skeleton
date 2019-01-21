#include "Application/Application.h"
#include "Input/Input.h"
#include "Compute/Dft.h"
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
	Application app(200);

	std::vector<float>real;
	std::vector<float>imag;
	std::vector<float>s(64);
	for (UINT i = 0; i < s.size(); ++i)
	{
		s[i] = 0.25f * sin(2.0f * 3.14159265f * 250.0f * i / 8000.0f);
	}

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
	}

	return 0;
}