#pragma once
#include "../etc/Vector2.h"
#include <string>

namespace help {
	// ユニコード文字に変換
	std::wstring ChangeWString(const std::string& st);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// キー入力
	bool CheckKey(const int& key);
}
