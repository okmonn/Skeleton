#pragma once
#include <string>

#define PI 3.14159265f
#define RAD(X) (X) * PI / 180.0f

namespace func
{
	// ���C�h������̕ϊ�
	std::wstring ChangeWString(const std::string& st);

	// ������̌���
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// �j���[�g���@
	float Newton(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop = 16);
}
