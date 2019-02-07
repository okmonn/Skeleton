#include "Useful.h"
#include <Windows.h>
#include <random>
#include <algorithm>

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
	byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), static_cast<int>(code.size()), &str[0], byteSize, nullptr, nullptr);

	return str;
}

// ���W�A���ϊ�
float use::Rad(const float & i)
{
	return (i * PIF) / 180.0f;
}

// �o�C�g�ϊ�
int use::Byte(const int & bit)
{
	return bit / 8;
}

// �C�ӂ̌�����؂�̂�
float use::Floor(const float & i, const int & num)
{
	float tmp = i;
	tmp *= std::pow(10.0f, -num - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, num + 1);

	return tmp;
}

// �C�ӂ̌�����l�̌ܓ�
float use::Round(const float & i, const int & num)
{
	float tmp = i;
	tmp *= std::pow(10.0f, -num - 1);
	tmp  = std::floor(tmp + 0.5f);
	tmp *= std::pow(10.0f, num + 1);

	return tmp;
}

// �����_���l�̎擾
int use::GetRand(const int & min, const int & max)
{
	std::random_device dev;
#ifdef _WIN64
	std::mt19937_64 mt(dev());
#else
	std::mt19937 mt(dev());
#endif

	std::uniform_int_distribution<> rand(min, max);

	return rand(mt);
}

// �����_���l�̎擾
float use::GetRand(const float & min, const float & max)
{
	std::random_device dev;
#ifdef _WIN64
	std::mt19937_64 mt(dev());
#else
	std::mt19937 mt(dev());
#endif

	std::uniform_real_distribution<float> rand(min, max);

	return rand(mt);
}

// �����̊m�F
bool use::CheckChar(const std::string & find, const unsigned char * data, const size_t & num)
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

// �L�[�̓��͂̊m�F
unsigned int use::CheckKey(const unsigned int & key)
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
	POINT point{};
	GetCursorPos(&point);
	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// �V���N���֐�
float use::Sinc(const float & i)
{
	return (i == 0.0f) ? 1.0f : std::sin(i) / i;
}

// �n�j���O���֐�
double use::Haninng(const unsigned int & i, const size_t & num)
{
	return (num % 2 == 0)
		? 0.5 - 0.5 * std::cos(2.0 * PI * i / num)
		: 0.5 - 0.5 * std::cos(2.0 * PI * (i + 0.5) / num);
}

// ���U�t�[���G�ϊ�
void use::DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag)
{
	real.assign(input.size(), 0.0);
	imag.assign(input.size(), 0.0);
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			auto re =  std::cos(2.0 * PI * i * n / input.size());
			auto im = -std::sin(2.0 * PI * i * n / input.size());

			real[i] += re * input[n];
			imag[i] += im * input[n];
		}
	}
}

//���U�t�[���G�ϊ�
void use::DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	real.assign(input.size(), 0.0);
	imag.assign(input.size(), 0.0);
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int n = 0; n < input.size(); ++n)
		{
			auto re =  std::cos(2.0f * PIF * i * n / input.size());
			auto im = -std::sin(2.0f * PIF * i * n / input.size());

			real[i] += re * input[n];
			imag[i] += im * input[n];
		}
	}
}

// �t���U�t�[���G�ϊ�
std::vector<double> use::IDFT(const std::vector<double>& real, const std::vector<double>& imag)
{
	unsigned int index = 0;

	std::vector<double>adap(real.size(), 0.0);
	std::for_each(adap.begin(), adap.end(), [&](double& i)->void {
		for (unsigned int n = 0; n < real.size(); ++n)
		{
			auto re = std::cos(2.0 * PI * index * n / real.size());
			auto im = std::sin(2.0 * PI * index*  n / imag.size());

			i += re * real[n] - im * imag[n];
		}
		i /= real.size();
		++index;
	});

	return adap;
}

// �t���U�t�[���G�ϊ�
std::vector<float> use::IDFT(const std::vector<float>& real, const std::vector<float>& imag)
{
	unsigned int index = 0;

	std::vector<float>adap(real.size(), 0.0);
	std::for_each(adap.begin(), adap.end(), [&](float& i)->void {
		for (unsigned int n = 0; n < real.size(); ++n)
		{
			auto re = std::cos(2.0f * PIF * index * n / real.size());
			auto im = std::sin(2.0f * PIF * index*  n / imag.size());

			i += re * real[n] - im * imag[n];
		}
		i /= real.size();
		++index;
	});

	return adap;
}

// �����t�[���G�ϊ�
void use::FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag)
{
	unsigned int stage = static_cast<unsigned int>(std::ceil(log2(input.size())));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	real = input;
	real.resize(num, 0.0);
	imag.resize(num, 0.0);

	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				auto p = std::pow(2, st - 1) * n;
				 
				auto re0 =  real[index1];
				auto im0 =  imag[index1];
				auto re1 =  real[index2];
				auto im1 =  imag[index2];
				auto re2 =  std::cos(2.0 * PI * p / num);
				auto im2 = -std::sin(2.0 * PI * p / num);

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

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(real[index[i]], real[i]);
			std::swap(imag[index[i]], imag[i]);
		}
	}
}

// �����t�[���G�ϊ�
void use::FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	unsigned int stage = static_cast<unsigned int>(std::ceil(log2(input.size())));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	real = input;
	real.resize(num, 0.0f);
	imag.resize(num, 0.0f);

	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				auto p = std::pow(2.0f, static_cast<float>(st - 1)) * n;

				auto re0 =  real[index1];
				auto im0 =  imag[index1];
				auto re1 =  real[index2];
				auto im1 =  imag[index2];
				auto re2 =  std::cos(2.0f * PIF * p / num);
				auto im2 = -std::sin(2.0f * PIF * p / num);

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

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(real[index[i]], real[i]);
			std::swap(imag[index[i]], imag[i]);
		}
	}
}

// �t�����t�[���G�ϊ�
std::vector<double> use::IFFT(const std::vector<double>& real, const std::vector<double>& imag, const unsigned int & outNum)
{
	unsigned int stage = static_cast<unsigned int>(std::log2(real.size()));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	std::vector<double>re = real;
	std::vector<double>im = imag;
	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				auto p = pow(2, st - 1) * n;

				auto re0 = re[index1];
				auto im0 = im[index1];
				auto re1 = re[index2];
				auto im1 = im[index2];
				auto re2 = std::cos(2.0 * PI * p / num);
				auto im2 = std::sin(2.0 * PI * p / num);

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

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int n = 0; n < index.size(); ++n)
	{
		if (index[n] > n)
		{
			std::swap(re[index[n]], re[n]);
			std::swap(im[index[n]], im[n]);
		}
		re[n] /= num;
		im[n] /= num;
	}

	return std::vector<double>(&re[0], &re[outNum]);
}

// �t�����t�[���G�ϊ�
std::vector<float> use::IFFT(const std::vector<float>& real, const std::vector<float>& imag, const unsigned int & outNum)
{
	unsigned int stage = static_cast<unsigned int>(std::log2(real.size()));
	unsigned int num   = static_cast<unsigned int>(std::pow(2, stage));

	std::vector<float>re = real;
	std::vector<float>im = imag;
	std::vector<unsigned int>index(num, 0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < static_cast<unsigned int>(std::pow(2, stage - st)); ++n)
			{
				unsigned int index1 = static_cast<unsigned int>(std::pow(2, stage - st + 1)) * i + n;
				unsigned int index2 = static_cast<unsigned int>(std::pow(2, stage - st)) + index1;

				auto p = pow(2.0f, static_cast<float>(st - 1)) * n;

				auto re0 = re[index1];
				auto im0 = im[index1];
				auto re1 = re[index2];
				auto im1 = im[index2];
				auto re2 = std::cos(2.0f * PIF * p / num);
				auto im2 = std::sin(2.0f * PIF * p / num);

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

			unsigned int m = static_cast<unsigned int>(std::pow(2, st - 1) + i);
			index[m] = index[i] + static_cast<unsigned int>(std::pow(2, stage - st));
		}
	}

	for (unsigned int n = 0; n < index.size(); ++n)
	{
		if (index[n] > n)
		{
			std::swap(re[index[n]], re[n]);
			std::swap(im[index[n]], im[n]);
		}
		re[n] /= num;
		im[n] /= num;
	}

	return std::vector<float>(&re[0], &re[outNum]);
}

// �U���̎擾
std::vector<double> use::Amplitude(const std::vector<double>& real, const std::vector<double>& imag)
{
	unsigned int index = 0;

	std::vector<double>adap(real.size());
	std::for_each(adap.begin(), adap.end(), [&](double& i)->void {
		i = std::sqrt(real[index] * real[index] + imag[index] * imag[index]);
		++index;
	});

	return adap;
}

// �U���̎擾
std::vector<float> use::Amplitude(const std::vector<float>& real, const std::vector<float>& imag)
{
	unsigned int index = 0;

	std::vector<float>adap(real.size());
	std::for_each(adap.begin(), adap.end(), [&](float& i)->void {
		i = std::sqrt(real[index] * real[index] + imag[index] * imag[index]);
		i = Round(i, -1);
		++index;
	});

	return adap;
}

// �ʑ��̎擾
std::vector<double> use::Phase(const std::vector<double>& real, const std::vector<double>& imag)
{
	unsigned int index = 0;

	std::vector<double>adap(real.size());
	std::for_each(adap.begin(), adap.end(), [&](double& i)->void {
		i = std::atan2(static_cast<int>(imag[index]), static_cast<int>(real[index]));
		++index;
	});

	return adap;
}

// �ʑ��̎擾
std::vector<float> use::Phase(const std::vector<float>& real, const std::vector<float>& imag)
{
	unsigned int index = 0;

	std::vector<float>adap(real.size());
	std::for_each(adap.begin(), adap.end(), [&](float& i)->void {
		i = static_cast<float>(std::atan2(static_cast<int>(imag[index]), static_cast<int>(real[index])));
		++index;
	});

	return adap;
}
