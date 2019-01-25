#pragma once
#include "../Vector2.h"
#include <string>

namespace use
{
	// ユニコード変換
	std::wstring ChangeCode(const std::string& code);

	// マルチバイトに変換
	std::string ChangeCode(const std::wstring& code);

	// キーの入力の確認
	int CheckKey(const unsigned int& key);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// 画面上のマウス座標の取得
	Vec2 GetMousePos(void);
}
