#pragma once
#include "../etc/tstring.h"
#include <list>
#include <memory>
#include <unordered_map>

class Window;
class Root;
class Font;

class FontMane
{
public:
	// コンストラクタ
	FontMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// デストラクタ
	FontMane();

	// フォントの変更
	void ChangeFont(void* font) {
		handle = font;
	}
	// フォントサイズの変更
	void ChangeSize(const unsigned int& size) {
		this->size = size;
	}

private:
	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// フォント
	void* handle;

	// フォントサイズ
	unsigned int size;

	// フォント
	std::list<std::shared_ptr<Font>>font;
};
