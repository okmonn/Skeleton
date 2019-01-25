#include "Useful.h"
#include <Windows.h>

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

// キーの入力の確認
int use::CheckKey(const unsigned int & key)
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
