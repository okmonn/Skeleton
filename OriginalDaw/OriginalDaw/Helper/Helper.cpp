#include "Helper.h"
#include <Windows.h>

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

// メインディスプレイの解像度の取得
Vec2 help::GetDisplayResolution(void)
{
	return Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

// キー入力
bool help::CheckKey(const int & key)
{
	if (GetKeyState(key) & 0x80)
	{
		return true;
	}

	return false;
}
