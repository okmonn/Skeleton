#pragma once
#include "../etc/tstring.h"
#include <memory>
#include <unordered_map>

class Root;

class FontMane
{
public:
	// �R���X�g���N�^
	FontMane(std::weak_ptr<Root>root);
	// �f�X�g���N�^
	FontMane();

	// �t�H���g�̕ύX
	void ChangeFont(void* font) {
		this->font = font;
	}
	// �t�H���g�T�C�Y�̕ύX
	void ChangeSize(const unsigned int& size) {
		this->size = size;
	}

private:
	// ���[�g
	std::weak_ptr<Root>root;

	// �t�H���g
	void* font;

	// �t�H���g�T�C�Y
	unsigned int size;
};
