#include "Helper.h"
#include <Windows.h>

// �~����
#define PI 3.14159265f

// �o�C�g�ϊ�
int help::Byte(const int & i)
{
	return i / 8;
}

// ���W�A���ϊ�
float help::Radian(const float & i)
{
	return (i * PI) / 180.0f;
}

// ���j�R�[�h�����ɕϊ�
std::wstring help::ChangeWString(const std::string & st)
{
	//�������̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// ������̌���
bool help::CheckChar(const std::string & find, const unsigned char * data, const size_t & num)
{
	unsigned int index = 0;
	for (size_t i = 0; i < num; ++i)
	{
		if (data[i] == find[index])
		{
			if (index + 1 >= find.size())
			{
				return true;
			}

			++index;
		}
	}

	return false;
}

// ���C���f�B�X�v���C�̉𑜓x�̎擾
Vec2 help::GetDisplayResolution(void)
{
	return Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

// �L�[����
int help::CheckKey(const int & key)
{
	return (GetKeyState(key) & 0x80);
}

// �S���̃L�[�����i�[
void help::GetKeys(int * key)
{
	SetKeyboardState((unsigned char*)key);
}

// �}�E�X���W�̎擾
Vec2 help::GetMousePos(void)
{
	POINT point;
	GetCursorPos(&point);
	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// �}�E�X���W�̃Z�b�g
void help::SetMousePos(const Vec2 & pos)
{
	SetCursorPos(pos.x, pos.y);
}
