#pragma once
#include "../etc/Vector2.h"
#include <string>

namespace help {
	// ���j�R�[�h�����ɕϊ�
	std::wstring ChangeWString(const std::string& st);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// �L�[����
	bool CheckKey(const int& key);
}
