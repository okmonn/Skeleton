#pragma once
#include "../etc/tstring.h"
#include <memory>
#include <unordered_map>

class FontLoader
{
public:
	// �f�X�g���N�^
	~FontLoader();

	// �C���X�^���X�ϐ��̎擾
	static FontLoader& Get(void) {
		static FontLoader instance;
		return instance;
	}

	// �t�H���g�f�[�^�̓ǂݍ���
	long Load(const std::tstring& fontName, const unsigned int& size);

	// �t�H���g�f�[�^�̎擾
	void* GetFont(const std::tstring& fontName, const unsigned int& size) {
		return font[fontName][size];
	}

private:
	// �R���X�g���N�^
	FontLoader();
	FontLoader(const FontLoader&) = delete;
	void operator=(const FontLoader&) = delete;


	// �t�H���g�f�[�^
	std::unordered_map<std::tstring, std::unordered_map<unsigned int, void*>>font;
};
