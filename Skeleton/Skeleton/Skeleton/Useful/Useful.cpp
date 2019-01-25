#include "Useful.h"
#include <Windows.h>

// ���j�R�[�h�ϊ�
std::wstring use::ChangeCode(const std::string & code)
{
	//�T�C�Y�̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// �}���`�o�C�g�ɕϊ�
std::string use::ChangeCode(const std::wstring & code)
{
	//�T�C�Y�̎擾
	auto byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), -1, nullptr, 0, nullptr, nullptr);

	std::string str;
	str.resize(byteSize);

	//�ϊ�
	byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), code.size(), &str[0], byteSize, nullptr, nullptr);

	return str;
}

// �L�[�̓��͂̊m�F
int use::CheckKey(const unsigned int & key)
{
	return (GetKeyState(key) & 0x80);
}

// ���C���f�B�X�v���C�̉𑜓x�̎擾
Vec2 use::GetDisplayResolution(void)
{
	return Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

// ��ʏ�̃}�E�X���W�̎擾
Vec2 use::GetMousePos(void)
{
	POINT point;
	GetCursorPos(&point);
	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}
