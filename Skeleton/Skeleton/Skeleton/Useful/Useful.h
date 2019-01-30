#pragma once
#include "../Vector2.h"
#include <string>

namespace use
{
	// ユニコード変換
	std::wstring ChangeCode(const std::string& code);

	// マルチバイトに変換
	std::string ChangeCode(const std::wstring& code);

	// 文字の確認
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// バイト変換
	int Byte(const int& bit);

	// キーの入力の確認
	unsigned int CheckKey(const unsigned int& key);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// 画面上のマウス座標の取得
	Vec2 GetMousePos(void);

	// シンク窓関数
	float Sinc(const float& i);
}
