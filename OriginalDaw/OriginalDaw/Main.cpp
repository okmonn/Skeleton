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

	const double PI = 3.14159265;

	snd::Info info{};
	info.channel = 1;
	info.sample  = 8000;
	info.bit     = 8;
	info.data.resize(info.sample * help::Byte(info.bit) * info.channel);
	info.data.resize(64);
	for (unsigned int i = 0; i < 64; ++i)
	{
		/*info.data[i] = 2.0f * sin(4.0f * (2.0f * PI / info.data.size()) * i)
			+ 3.0f * cos(2.0f * (2.0f * PI / info.data.size()) * i);*/
	}
	std::vector<float>real;
	std::vector<float>imag;
	std::vector<float>s(64);
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		s[i] = 0.25 * sin(2.0 * PI * 250.0 * i / 8000.0);
	}
	DFT dft(s.size());
	dft.Execution(s, real, imag);
	for (unsigned int i = 0; i < info.data.size(); ++i)
	{
		double a = sqrt(real[i] * real[i] + imag[i] * imag[i]);
		double b = atan(imag[i] / real[i]);
		printf("%d：%f：%f：%f：%f\n", i, real[i], imag[i], a, b);
	}
	/*help::DFT(s, real, imag);
	for (unsigned int i = 0; i < info.data.size(); ++i)
	{
		double a = sqrt(real[i] * real[i] + imag[i] * imag[i]);
		double b = atan(imag[i] / real[i]);
		printf("%d：%f：%f：%f：%f\n", i, real[i], imag[i], a, b);
	}
	auto tmp = help::IDFT(real, imag);
	for (unsigned int i = 0; i < tmp.size(); ++i)
	{
		printf("%d：%f：%f\n", i, s[i], tmp[i]);
	}*/

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