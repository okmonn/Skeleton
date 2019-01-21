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
	Application app(winSize);

	const float PI = 3.14159265f;

	auto haninng = [&](const unsigned int& i, const unsigned int& num)->float {
		float tmp = 0.0f;

		tmp = (num % 2 == 0) ?
			//偶数
			0.5f - 0.5f * cos(2.0f * PI * i / num) :
			//奇数
			0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / num);

		return tmp;
	};

	snd::Info info{};
	info.channel = 1;
	info.sample  = 8000;
	info.bit     = 8;
	info.data.resize(info.sample * help::Byte(info.bit) * info.channel);
	info.data.resize(64);
	for (unsigned int i = 0; i < 64; ++i)
	{
		info.data[i] = 0.25f * sin(2.0f * PI * 250.0f * i / info.sample);
		/*info.data[i] = 2.0f * sin(4.0f * (2.0f * PI / info.data.size()) * i)
			+ 3.0f * cos(2.0f * (2.0f * PI / info.data.size()) * i);*/
	}
	std::vector<float>real;
	std::vector<float>imag;
	help::DFT(info.data, real, imag);
	for (unsigned int i = 0; i < real.size(); ++i)
	{
		float a = sqrt(real[i] * real[i] + imag[i] * imag[i]);
		float b = atan(imag[i] / real[i]);
		printf("%d：%f：%f：%f：%f\n", i, real[i], imag[i], a, b);
	}
	auto tmp = help::IDFT(real, imag);
	for (unsigned int i = 0; i < real.size(); ++i)
	{
		printf("%d：%f：%f\n", i, info.data[i], tmp[i]);
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