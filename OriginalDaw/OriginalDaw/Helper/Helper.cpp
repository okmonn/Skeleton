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

// �n�j���O��
float help::Haninng(const unsigned int & i, const unsigned int & num)
{
	return (num % 2 == 0)
		? 0.5f - 0.5f * cos(2.0f * PI * i / num)
		: 0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / num);
}

// ���U�t�[���G�ϊ�
void help::DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real.resize(input.size(), 0.0f);
	imag.resize(input.size(), 0.0f);

	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			float re =  cos(2.0f * PI * i * n / real.size());
			float im = -sin(2.0f * PI * i * n / imag.size());

			real[i] += re * input[n];
			imag[i] += im * input[n];
		}
	}
}

// �t���U�t�[���G�ϊ�
std::vector<float> help::IDFT(const std::vector<float>& real, const std::vector<float>& imag)
{
	std::vector<float>output(real.size(), 0.0f);

	for (unsigned int i = 0; i < output.size(); ++i)
	{
		for (unsigned int n = 0; n < output.size(); ++n)
		{
			float re = cos(2.0f * PI * i * n / real.size());
			float im = sin(2.0f * PI * i * n / imag.size());

			output[i] += re * real[n] * Haninng(i, real.size()) - im * imag[n];
		}

		output[i] /= real.size();
	}

	return output;
}

// �����t�[���G�ϊ�
void help::FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	unsigned int stage = static_cast<unsigned int>(ceil(log2(input.size())));
	unsigned int num = static_cast<unsigned int>(pow(2, stage));

	real = input;
	real.resize(num);
	imag.assign(num, 0.0f);

	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(pow(2, stage - st)) + index1;

				float p = static_cast<float>(pow(2, st - 1) * n);

				float re0 =  real[index1];
				float im0 =  imag[index1];
				float re1 =  real[index2];
				float im1 =  imag[index2];
				float re2 =  cos(2.0f * PI * p / num);
				float im2 = -sin(2.0f * PI * p / num);

				real[index1] = re0 + re1;
				imag[index1] = im0 + im1;
				if (st < stage)
				{
					real[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					imag[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					real[index2] = re0 - re1;
					imag[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(pow(2, stage - st));
		}
	}

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (index[i] > i)
		{
			float re = real[index[i]];
			float im = imag[index[i]];
			real[index[i]] = real[i];
			imag[index[i]] = imag[i];
			real[i] = re;
			imag[i] = im;
		}
	}
}

// �t�����t�[���G�ϊ�
std::vector<float> help::IFFT(const std::vector<float>& real, std::vector<float>& imag, const unsigned int & outNum)
{
	unsigned int stage = static_cast<unsigned int>(log2(real.size()));
	unsigned int num = static_cast<unsigned int>(pow(2, stage));

	std::vector<float>re = real;
	std::vector<float>im = imag;
	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(pow(2, stage - st)) + index1;

				float p = static_cast<float>(pow(2, st - 1) * n);

				float re0 = re[index1];
				float im0 = im[index1];
				float re1 = re[index2];
				float im1 = im[index2];
				float re2 = cos(2.0f * PI * p / num);
				float im2 = sin(2.0f * PI * p / num);

				re[index1] = re0 + re1;
				im[index1] = im0 + im1;
				if (st < stage)
				{
					re[index2] = (re0 - re1) * re2 - (im0 - im1) * im2;
					im[index2] = (im0 - im1) * re2 + (re0 - re1) * im2;
				}
				else
				{
					re[index2] = re0 - re1;
					im[index2] = im0 - im1;
				}
			}

			unsigned int m = static_cast<unsigned int>(pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(pow(2, stage - st));
		}
	}

	for (unsigned int n = 0; n < index.size(); ++n)
	{
		if (index[n] > n)
		{
			float r = re[index[n]];
			float i = im[index[n]];
			re[index[n]] = re[n];
			im[index[n]] = im[n];
			re[n] = r;
			im[n] = i;
		}
		re[n] /= num;
		im[n] /= num;
	}

	return std::vector<float>(&re[0], &re[outNum]);
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
