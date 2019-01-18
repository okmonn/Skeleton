#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <vector>

namespace help {
	// �o�C�g�ϊ�
	int Byte(const int& i);

	// ���W�A���ϊ�
	float Radian(const float& i);

	// ���j�R�[�h�����ɕϊ�
	std::wstring ChangeWString(const std::string& st);

	// ������̌���
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// �L�[����
	int CheckKey(const int& key);

	// �S���̃L�[�����i�[
	void GetKeys(int* key);

	// �}�E�X���W�̎擾
	Vec2 GetMousePos(void);

	// �}�E�X���W�̃Z�b�g
	void SetMousePos(const Vec2& pos);
}
