#include "Helper.h"
#include <Windows.h>

// 円周率
#define PI 3.14159265f

// バイト変換
int help::Byte(const int & i)
{
	return i / 8;
}

// ラジアン変換
float help::Radian(const float & i)
{
	return (i * PI) / 180.0f;
}

// ユニコード文字に変換
std::wstring help::ChangeWString(const std::string & st)
{
	//文字数の取得
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// 文字列の検索
bool help::CheckChar(const std::string & find, const unsigned char * data, const size_t & num)
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

// ハニング窓関数
float help::Haninng(const unsigned int & i, const size_t & size)
{
	float tmp = 0.0f;

	tmp = (size % 2 == 0) ?
		//偶数
		0.5f - 0.5f * std::cosf(2.0f * PI * i / size) :
		//奇数
		0.5f - 0.5f * std::cosf(2.0f * PI * (i + 0.5f) / size);

	if (tmp == 0.0f)
	{
		tmp = 1.0f;
	}

	return tmp;
}

// シンク関数
float help::Sinc(const float & i)
{
	return (i == 0.0f) ? 1.0f : std::sinf(i) / i;
}

// 離散フーリエ変
void help::DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real = input;
	imag.assign(input.size(), 0.0f);

	float tmpR = 0.0f;
	float tmpI = 0.0f;
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			tmpR =  std::cosf(2.0f * PI * i * n / input.size());
			tmpI = -std::sinf(2.0f * PI * i * n / input.size());

			real[i] += tmpR * (input[n] * Haninng(n, input.size())) - tmpI * 0.0f;
			imag[i] += tmpR * 0.0f                                  + tmpI * (input[n] * Haninng(n, input.size()));
		}
	}
}

// 逆離散フーリエ変換
std::vector<float> help::IDFT(const std::vector<float>& real, const std::vector<float>& imag)
{
	std::vector<float>out(real.size(), 0.0f);

	float tmpR = 0.0f;
	float tmpI = 0.0f;
	for (unsigned int i = 0; i < real.size(); ++i)
	{
		float tmp = 0.0f;
		for (unsigned int n = 0; n < real.size(); ++i)
		{
			tmpR = std::cosf(2.0f * PI * i * n / real.size());
			tmpI = std::sinf(2.0f * PI * i * n / real.size());

			out[i] += (tmpR * real[n] - tmpI * imag[n]) / real.size();
			tmp    += (tmpR * imag[n] + tmpI * real[n]) / real.size();
		}

		out[i] /= Haninng(i, real.size());
		tmp    /= Haninng(i, real.size());
	}

	return out;
}

// 高速フーリエ変換
void help::FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real.assign(input.begin(), input.end());
	imag.assign(input.size(), 0.0f);

	std::vector<float>re(3, 0.0f);
	std::vector<float>im(3, 0.0f);

	std::vector<unsigned int>tbl(input.size(), 0);

	unsigned int stage = static_cast<unsigned int>(std::log2(input.size()));
	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(st - 1, 2)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(stage - st, 2)); ++n)
			{
				auto index1 = std::pow(stage - st + 1, 2) * i + n;
				auto index2 = std::pow(stage - st, 2) + index1;

				auto p = std::powf(static_cast<float>(st - 1), 2.0f) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] =  std::cosf((2.0f * PI * p) / input.size());
				im[2] = -std::sinf((2.0f * PI * p) / input.size());

				if (st < stage)
				{
					real[index1] =  re[0] + re[1];
					imag[index1] =  im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//インデックスの格納
			tbl[std::pow(st - 1, 2) + i] = tbl[i] + static_cast<unsigned int>(std::pow(stage - st, 2));
		}
	}

	//並び替え
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		if (tbl[i] > i)
		{
			auto re      = real[tbl[i]];
			auto im      = imag[tbl[i]];
			real[tbl[i]] = real[i];
			imag[tbl[i]] = imag[i];
			real[i]      = re;
			imag[i]      = im;
		}
	}
}

// 逆高速フーリエ変換
std::vector<float> help::IFFT(const std::vector<float>& inputReal, const std::vector<float>& inputImag)
{
	std::vector<float>real(inputReal.begin(), inputReal.end());
	std::vector<float>imag(inputImag.begin(), inputImag.end());

	std::vector<float>re(3, 0.0f);
	std::vector<float>im(3, 0.0f);

	std::vector<unsigned int>tbl(real.size(), 0);

	unsigned int stage = static_cast<unsigned int>(std::log2(real.size()));
	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(st - 1, 2)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(stage - st, 2)); ++n)
			{
				auto index1 = std::pow(stage - st + 1, 2) * i + n;
				auto index2 = std::pow(stage - st, 2) + index1;

				auto p = std::powf(static_cast<float>(st - 1), 2.0f) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] = std::cosf((2.0f * PI * p) / real.size());
				im[2] = std::sinf((2.0f * PI * p) / real.size());

				if (st < stage)
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//インデックスの格納
			tbl[std::pow(st - 1, 2) + i] = tbl[i] + static_cast<unsigned int>(std::pow(stage - st, 2));
		}
	}

	//並び替え
	for (unsigned int i = 0; i < real.size(); ++i)
	{
		if (tbl[i] > i)
		{
			auto re      = real[tbl[i]];
			auto im      = imag[tbl[i]];
			real[tbl[i]] = real[i];
			imag[tbl[i]] = imag[i];
			real[i]      = re;
			imag[i]      = im;
		}

		real[i] /= static_cast<float>(real.size());
		imag[i] /= static_cast<float>(real.size());
	}

	return real;
}

// メインディスプレイの解像度の取得
Vec2 help::GetDisplayResolution(void)
{
	return Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

// キー入力
int help::CheckKey(const int & key)
{
	return (GetKeyState(key) & 0x80);
}

// 全部のキー情報を格納
void help::GetKeys(int * key)
{
	SetKeyboardState((unsigned char*)key);
}

// マウス座標の取得
Vec2 help::GetMousePos(void)
{
	POINT point;
	GetCursorPos(&point);
	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// マウス座標のセット
void help::SetMousePos(const Vec2 & pos)
{
	SetCursorPos(pos.x, pos.y);
}
