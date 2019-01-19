#include "Application/Application.h"
#include "Input/Input.h"
#include "Compute/Fourier.h"
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
	Fourier f(L"Shader/Fourier.hlsl");

	const float PI = 3.14159265f;
	std::vector<float>real;
	std::vector<float>imag;

	auto fft = [&](const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)->void{
		unsigned int radix = (UINT)ceil(log2((float)input.size()));
		unsigned int size = (UINT)pow(2, radix);

		real = input;
		real.resize(size);
		imag.assign(size, 0.0f);

		std::vector<unsigned int>index(size, 0);

		unsigned int stage = radix;
		for (unsigned int st = 1; st <= stage; ++st)
		{
			for (unsigned int i = 0; i < (UINT)pow(2, st - 1); ++i)
			{
				for (unsigned int n = 0; n < (UINT)pow(2, stage - st); ++n)
				{
					unsigned int index1 = (UINT)pow(2, stage - st + 1) * i + n;
					unsigned int index2 = (UINT)pow(2, stage - st) + index1;
					
					float p = (float)pow(2, st - 1) * n;

					float re0 =  real[index1];
					float im0 =  imag[index1];
					float re1 =  real[index2];
					float im1 =  imag[index2];
					float re2 =  cos((2.0f * 3.14159265f * p) / size);
					float im2 = -sin((2.0f * 3.14159265f * p) / size);

					if (st < stage)
					{
						real[index1] = re0 + re1;
						imag[index1] = im0 + im1;
						real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
						imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
					}
					else
					{
						real[index1] = re0 + re1;
						imag[index1] = im0 + im1;
						real[index2] = re0 - re1;
						imag[index2] = im0 - im1;
					}
				}

				index[(UINT)pow(2, st - 1) + i] = index[i] + (UINT)pow(2, stage - st);				
			}
		}

		for (unsigned int i = 0; i < index.size(); ++i)
		{
			if (index[i] > i)
			{
				float re = real[index[i]];
				float im = imag[index[i]];
				real[index[i]] = real[i];
				imag[index[i]] = imag[i];
				real[i] = re;
				imag[i] = im;
			}
		}
	};

	auto ifft = [&](std::vector<float>& real, std::vector<float>& imag, const unsigned int num)->void {
		unsigned int size = real.size();

		std::vector<unsigned int>index(size, 0);

		unsigned int stage = (UINT)log2(size);
		for (unsigned int st = 1; st <= stage; ++st)
		{
			for (unsigned int i = 0; i < (UINT)pow(2, st - 1); ++i)
			{
				for (unsigned int n = 0; n < (UINT)pow(2, stage - st); ++n)
				{
					unsigned int index1 = (UINT)pow(2, stage - st + 1) * i + n;
					unsigned int index2 = (UINT)pow(2, stage - st) + index1;

					float p = (float)pow(2, st - 1) * n;

					float re0 = real[index1];
					float im0 = imag[index1];
					float re1 = real[index2];
					float im1 = imag[index2];
					float re2 = cos((2.0f * 3.14159265f * p) / size);
					float im2 = sin((2.0f * 3.14159265f * p) / size);

					if (st < stage)
					{
						real[index1] = re0 + re1;
						imag[index1] = im0 + im1;
						real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
						imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
					}
					else
					{
						real[index1] = re0 + re1;
						imag[index1] = im0 + im1;
						real[index2] = re0 - re1;
						imag[index2] = im0 - im1;
					}
				}

				index[(UINT)pow(2, st - 1) + i] = index[i] + (UINT)pow(2, stage - st);
			}
		}

		for (unsigned int i = 0; i < index.size(); ++i)
		{
			if (index[i] > i)
			{
				float re = real[index[i]];
				float im = imag[index[i]];
				real[index[i]] = real[i];
				imag[index[i]] = imag[i];
				real[i] = re;
				imag[i] = im;
			}
			real[i] /= size;
			imag[i] /= size;
		}

		real.resize(num);
		imag.resize(num);
	};
	

	snd::Info info{};
	info.channel = 1;
	info.sample  = 44100;
	info.bit     = 8;
//	info.data.resize(info.sample * help::Byte(info.bit) * info.channel);
	info.data.resize(500);
	for (unsigned int i = 0; i < info.data.size(); ++i)
	{
		info.data[i] = 1.0f * sin((2.0f * PI * 440.0f * i) / info.sample);
	}
	fft(info.data, real, imag);
	ifft(real, imag, info.data.size());
	//info.data = real;
	for (unsigned int i = 0; i < info.data.size(); ++i)
	{
		if(info.data[i] != real[i])
		printf("%d:%f：%f\n", i, info.data[i], real[i]);
	}
	/*f.Init(info.data.size());
	f.Execution(info.data, real, imag);
	f.ChangeType(FourierType::IDFT);
	f.Execution(info.data, real, imag);*/
	Sound s(info);
	s.Play(false);

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