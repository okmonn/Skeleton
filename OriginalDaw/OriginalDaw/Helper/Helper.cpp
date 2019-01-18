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
