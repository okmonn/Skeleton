#include "Func.h"
#include <Windows.h>

// ���C�h������̕ϊ�
std::wstring func::ChangeWString(const std::string & st)
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
std::string func::FindString(const std::string & path, const char & find, const unsigned int & offset, const bool & end)
{
	unsigned int pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// �j���[�g���@
float func::Newton(const float & input, const float & pos1X, const float & pos1Y, const float & pos2X, const float & pos2Y, const unsigned int & loop)
{
	if (pos1X == pos1Y && pos2X == pos2Y)
	{
		//�����Ȃ̂Ōv�Z�s�v
		return input;
	}

	float t = input;
	//t^3�̌W��
	float k3 = 1 + (3 * pos1X) - (3 * pos2X);
	//t^2�̌W��
	float k2 = (3 * pos2X) - (6 * pos1X);
	//t�̌W��
	float k1 = 3 * pos1X;

	const float epsilon = 0.0005f;

	for (unsigned int i = 0; i < loop; ++i)
	{
		//f(x)
		float ft = (t * t * t * k3) + (t * t * k2) + (t * k1) - input;
		if (ft <= epsilon && ft >= -epsilon)
		{
			break;
		}
		//f(x)�̔�������
		float fdt = (3 * t * t * k3) + (2 * t * k2) + k1;
		if (fdt == 0.0f)
		{
			break;
		}
		t = t - ft / fdt;
	}

	//���]
	float reverse = (1.0f - t);

	return (3 * reverse * reverse * t * pos1Y) +
		(3 * reverse * t * t * pos2Y) +
		(t * t * t);
}
