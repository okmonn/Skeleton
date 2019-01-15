#include "Helper.h"
#include <Windows.h>

// 円周率
#define PI 3.14159265f

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
void help::IDFT(const std::vector<float>& real, const std::vector<float>& imag, std::vector<float>& out)
{
	out.assign(real.size(), 0.0f);

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
