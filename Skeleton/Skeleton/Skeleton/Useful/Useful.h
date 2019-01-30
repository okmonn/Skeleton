#pragma once
#include "../Vector2.h"
#include <string>

namespace use
{
	// ���j�R�[�h�ϊ�
	std::wstring ChangeCode(const std::string& code);

	// �}���`�o�C�g�ɕϊ�
	std::string ChangeCode(const std::wstring& code);

	// �����̊m�F
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// �o�C�g�ϊ�
	int Byte(const int& bit);

	// �L�[�̓��͂̊m�F
	unsigned int CheckKey(const unsigned int& key);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// ��ʏ�̃}�E�X���W�̎擾
	Vec2 GetMousePos(void);

	// �V���N���֐�
	float Sinc(const float& i);
}
