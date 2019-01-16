#pragma once
#include "../etc/tstring.h"
#include <memory>
#include <unordered_map>

class FontLoader
{
public:
	// デストラクタ
	~FontLoader();

	// インスタンス変数の取得
	static FontLoader& Get(void) {
		static FontLoader instance;
		return instance;
	}

	// フォントデータの読み込み
	long Load(const std::tstring& fontName, const unsigned int& size);

	// フォントデータの取得
	void* GetFont(const std::tstring& fontName, const unsigned int& size) {
		return font[fontName][size];
	}

private:
	// コンストラクタ
	FontLoader();
	FontLoader(const FontLoader&) = delete;
	void operator=(const FontLoader&) = delete;


	// フォントデータ
	std::unordered_map<std::tstring, std::unordered_map<unsigned int, void*>>font;
};
