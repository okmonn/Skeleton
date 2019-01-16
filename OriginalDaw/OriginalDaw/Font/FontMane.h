#pragma once
#include "../etc/tstring.h"
#include <memory>
#include <unordered_map>

class Root;

class FontMane
{
public:
	// コンストラクタ
	FontMane(std::weak_ptr<Root>root);
	// デストラクタ
	FontMane();

	// フォントの変更
	void ChangeFont(void* font) {
		this->font = font;
	}
	// フォントサイズの変更
	void ChangeSize(const unsigned int& size) {
		this->size = size;
	}

private:
	// ルート
	std::weak_ptr<Root>root;

	// フォント
	void* font;

	// フォントサイズ
	unsigned int size;
};
