#pragma once
#include <string>
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
	long Load(const std::string& fontName, const unsigned int& size);

	// フォントデータの取得
	void* GetFont(const std::string& fontName, const unsigned int& size) {
		return font[fontName][size];
	}

private:
	// コンストラクタ
	FontLoader();
	FontLoader(const FontLoader&) = delete;
	void operator=(const FontLoader&) = delete;


	// フォントデータ
	std::unordered_map<std::string, std::unordered_map<unsigned int, void*>>font;
};
