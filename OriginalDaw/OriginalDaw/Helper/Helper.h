#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <vector>

namespace help {
	// バイト変換
	int Byte(const int& i);

	// ラジアン変換
	float Radian(const float& i);

	// ユニコード文字に変換
	std::wstring ChangeWString(const std::string& st);

	// 文字列の検索
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// キー入力
	int CheckKey(const int& key);

	// 全部のキー情報を格納
	void GetKeys(int* key);

	// マウス座標の取得
	Vec2 GetMousePos(void);

	// マウス座標のセット
	void SetMousePos(const Vec2& pos);
}
