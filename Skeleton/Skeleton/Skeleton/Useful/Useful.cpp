#include "Useful.h"
#include <Windows.h>

// 円周率
#define PI 3.14159265f

// ユニコード変換
std::wstring use::ChangeCode(const std::string & code)
{
	//サイズの取得
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// マルチバイトに変換
std::string use::ChangeCode(const std::wstring & code)
{
	//サイズの取得
	auto byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), -1, nullptr, 0, nullptr, nullptr);

	std::string str;
	str.resize(byteSize);

	//変換
	byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), code.size(), &str[0], byteSize, nullptr, nullptr);

	return str;
}

// ラジアン変換
float use::Rad(const float & i)
{
	return (i * PI) / 180.0f;
}

// バイト変換
int use::Byte(const int & bit)
{
	return bit / 8;
}

// 任意の桁から切り捨て
float use::Floor(const float & i, const int & num)
{
	float tmp = i;
	tmp *= std::pow(10.0f, -num - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, num + 1);

	return tmp;
}

// 任意の桁から四捨五入
float use::Round(const float & i, const int & num)
{
	float tmp = i;
	tmp *= std::pow(10.0f, -num - 1);
	tmp  = std::floor(tmp + 0.5f);
	tmp *= std::pow(10.0f, num + 1);

	return tmp;
}

// 文字の確認
bool use::CheckChar(const std::string & find, const unsigned char * data, const size_t & num)
{
	unsigned int index = 0;
	for (size_t i = 0; i < num; ++i)
	{
		if (data[i] == find[index])
		{
			if (index + 1 >= find.size())
			{
				return true;
			}

			++index;
		}
	}

	return false;
}

// キーの入力の確認
unsigned int use::CheckKey(const unsigned int & key)
{
	return (GetKeyState(key) & 0x80);
}

// メインディスプレイの解像度の取得
Vec2 use::GetDisplayResolution(void)
{
	return Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

// 画面上のマウス座標の取得
Vec2 use::GetMousePos(void)
{
	POINT point;
	GetCursorPos(&point);
	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// シンク窓関数
float use::Sinc(const float & i)
{
	return (i == 0.0f) ? 1.0f : std::sin(i) / i;
}

// ハニング窓関数
float use::Haninng(const unsigned int & i, const size_t & num)
{
	return (num % 2 == 0)
		? 0.5 - 0.5 * std::cos(2.0 * PI * i / num)
		: 0.5 - 0.5 * std::cos(2.0 * PI * (i + 0.5) / num);
}

// 離散フーリエ変換
void use::DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag)
{
	real.resize(input.size());
	imag.resize(input.size());

	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			double re =  std::cos(2.0 * static_cast<double>(PI) * i * n / real.size());
			double im = -std::sin(2.0 * static_cast<double>(PI) * i * n / imag.size());

			real[i] += re * input[n];
			imag[i] += im * input[n];
			//real[i] += re * input[n] * Haninng(n, real.size());
			//imag[i] += im * input[n] * Haninng(n, imag.size());
		}
	}
}

//離散フーリエ変換・精度低下
void use::DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real.resize(input.size());
	imag.resize(input.size());

	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			float re =  std::cos(2.0f * PI * i * n / real.size());
			float im = -std::sin(2.0f * PI * i * n / imag.size());

			real[i] += re * input[n];
			imag[i] += im * input[n];
			//real[i] += re * input[n] * Haninng(n, real.size());
			//imag[i] += im * input[n] * Haninng(n, imag.size());
		}
	}
}

// 逆離散フーリエ変換
std::vector<double> use::IDFT(const std::vector<double>& real, const std::vector<double>& imag)
{
	std::vector<double>output(real.size());

	for (unsigned int i = 0; i < output.size(); ++i)
	{
		for (unsigned int n = 0; n < output.size(); ++n)
		{
			double re = std::cos(2.0 * static_cast<double>(PI) * i * n / real.size());
			double im = std::sin(2.0 * static_cast<double>(PI) * i * n / imag.size());

			output[i] += re * real[n] - im * imag[n];
		}

		output[i] /= real.size();
		//output[i] /= real.size() * Haninng(i, output.size());
	}

	return output;
}

// 逆離散フーリエ変換・精度低下
std::vector<float> use::IDFT(const std::vector<float>& real, const std::vector<float>& imag)
{
	std::vector<float>output(real.size());

	for (unsigned int i = 0; i < output.size(); ++i)
	{
		for (unsigned int n = 0; n < output.size(); ++n)
		{
			float re = std::cos(2.0f * PI * i * n / real.size());
			float im = std::sin(2.0f * PI * i * n / imag.size());

			output[i] += re * real[n] - im * imag[n];
		}

		output[i] /= real.size();
		//output[i] /= real.size() * Haninng(i, output.size());
	}

	return output;
}

// 高速フーリエ変換
void use::FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag)
{
	unsigned int stage = static_cast<unsigned int>(std::ceil(log2(input.size())));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	real = input;
	real.resize(num);
	imag.resize(num);

	std::vector<unsigned int>index(num);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				double p = std::pow(2, st - 1) * n;

				double re0 = real[index1];
				double im0 = imag[index1];
				double re1 = real[index2];
				double im1 = imag[index2];
				double re2 =  std::cos(2.0 * static_cast<double>(PI) * p / num);
				double im2 = -std::sin(2.0 * static_cast<double>(PI) * p / num);

				real[index1] = re0 + re1;
				imag[index1] = im0 + im1;
				if (st < stage)
				{
					real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					real[index2] = re0 - re1;
					imag[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(real[index[i]], real[i]);
			std::swap(imag[index[i]], imag[i]);
		}
	}
}

// 高速フーリエ変換・精度低下
void use::FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	unsigned int stage = static_cast<unsigned int>(std::ceil(log2(input.size())));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	real = input;
	real.resize(num);
	imag.resize(num);

	std::vector<unsigned int>index(num);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				float p = std::pow(2.0f, st - 1) * n;

				float re0 = real[index1];
				float im0 = imag[index1];
				float re1 = real[index2];
				float im1 = imag[index2];
				float re2 =  std::cos(2.0f * PI * p / num);
				float im2 = -std::sin(2.0f * PI * p / num);

				real[index1] = re0 + re1;
				imag[index1] = im0 + im1;
				if (st < stage)
				{
					real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					real[index2] = re0 - re1;
					imag[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(real[index[i]], real[i]);
			std::swap(imag[index[i]], imag[i]);
		}
	}
}

// 逆高速フーリエ変換
std::vector<double> use::IFFT(const std::vector<double>& real, const std::vector<double>& imag, const unsigned int & outNum)
{
	unsigned int stage = static_cast<unsigned int>(std::log2(real.size()));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	std::vector<double>re = real;
	std::vector<double>im = imag;
	std::vector<unsigned int>index(num);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				double p = pow(2, st - 1) * n;

				double re0 = re[index1];
				double im0 = im[index1];
				double re1 = re[index2];
				double im1 = im[index2];
				double re2 = std::cos(2.0 * static_cast<double>(PI) * p / num);
				double im2 = std::sin(2.0 * static_cast<double>(PI) * p / num);

				re[index1] = re0 + re1;
				im[index1] = im0 + im1;
				if (st < stage)
				{
					re[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					im[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					re[index2] = re0 - re1;
					im[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int n = 0; n < index.size(); ++n)
	{
		if (index[n] > n)
		{
			std::swap(re[index[n]], re[n]);
			std::swap(im[index[n]], im[n]);
		}
		re[n] /= num;
		im[n] /= num;
	}

	return std::vector<double>(&re[0], &re[outNum - 1]);
}

// 逆高速フーリエ変換・精度低下
std::vector<float> use::IFFT(const std::vector<float>& real, const std::vector<float>& imag, const unsigned int & outNum)
{
	unsigned int stage = static_cast<unsigned int>(std::log2(real.size()));
	unsigned int num = static_cast<unsigned int>(std::pow(2, stage));

	std::vector<float>re = real;
	std::vector<float>im = imag;
	std::vector<unsigned int>index(num);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				float p = pow(2.0f, st - 1) * n;

				float re0 = re[index1];
				float im0 = im[index1];
				float re1 = re[index2];
				float im1 = im[index2];
				float re2 = std::cos(2.0f * PI * p / num);
				float im2 = std::sin(2.0f * PI * p / num);

				re[index1] = re0 + re1;
				im[index1] = im0 + im1;
				if (st < stage)
				{
					re[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					im[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					re[index2] = re0 - re1;
					im[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int n = 0; n < index.size(); ++n)
	{
		if (index[n] > n)
		{
			std::swap(re[index[n]], re[n]);
			std::swap(im[index[n]], im[n]);
		}
		re[n] /= num;
		im[n] /= num;
	}

	return std::vector<float>(&re[0], &re[outNum - 1]);
}
