#pragma once
#include "../Vector2.h"
#include <string>

namespace use
{
	// ���j�R�[�h�ϊ�
	std::wstring ChangeCode(const std::string& code);

	// �}���`�o�C�g�ɕϊ�
	std::string ChangeCode(const std::wstring& code);

	// �L�[�̓��͂̊m�F
	int CheckKey(const unsigned int& key);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// ��ʏ�̃}�E�X���W�̎擾
	Vec2 GetMousePos(void);
}
