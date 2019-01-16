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
	// �R���X�g���N�^
	FontMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// �f�X�g���N�^
	FontMane();

	// �t�H���g�̕ύX
	void ChangeFont(void* font) {
		handle = font;
	}
	// �t�H���g�T�C�Y�̕ύX
	void ChangeSize(const unsigned int& size) {
		this->size = size;
	}

private:
	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �t�H���g
	void* handle;

	// �t�H���g�T�C�Y
	unsigned int size;

	// �t�H���g
	std::list<std::shared_ptr<Font>>font;
};
