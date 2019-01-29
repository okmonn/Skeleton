#include "Shifter.h"
#include <algorithm>

// �R���X�g���N�^
Shifter::Shifter()
{
}

// �f�X�g���N�^
Shifter::~Shifter()
{
}

// ������
std::vector<float> Shifter::FastForward(const std::vector<float>& input, const float & rate, const int & sample)
{
	/*//�T�C�Y�␳
	unsigned int size = static_cast<unsigned int>(input.size() / rate) + 1;
	if (size % 2 != 0)
	{
		++size;
	}
	//�K���f�[�^
	std::vector<float>tmp(size);

	//��l(10ms)
	const unsigned int tmpSize = static_cast<unsigned int>(sample * 0.01f);

	//�ŏ��l(5ms)
	const unsigned int min = static_cast<unsigned int>(sample * 0.005f);
	//�ő�l(20ms)
	const unsigned int max = static_cast<unsigned int>(sample * 0.02f);

	std::vector<float>x(tmpSize);
	std::vector<float>y(tmpSize);
	std::vector<float>r(max + 1);

	//�����
	unsigned int offset0 = 0;
	unsigned int offset1 = 0;
	while (offset0 + max * 2 < input.size())
	{
		for (unsigned int i = 0; i < tmpSize; ++i)
		{
			x[i] = input[offset0 + i];
		}

		float max_r = 0.0f;
		auto p = min;
		for (unsigned int i = min; i <= max; ++i)
		{
			for (unsigned int n = 0; n < tmpSize; ++n)
			{
				y[n] = input[offset0 + i + n];
			}

			r[i] = 0.0f;
			for (unsigned int n = 0; n < tmpSize; ++n)
			{
				r[i] += x[n] * y[n];
			}
			if (r[i] > max_r)
			{
				max_r = r[i];
				p = i;
			}
		}

		for (unsigned int i = 0; i < p; ++i)
		{
			tmp[offset1 + i] = input[offset0 + i] * (p - i) / p;
			tmp[offset1 + i] += input[offset0 + p + i] * i / p;
		} 

		unsigned int q = static_cast<unsigned int>(p / (rate - 1.0f) + 0.5f);
		for (unsigned int i = p; i < q; ++i)
		{
			if (offset0 + p + i >= input.size())
			{
				break;
			}
			tmp[offset1 + i] = input[offset0 + p + i];
		}

		offset0 += p + q;
		offset1 += q;
	}

	return tmp;*/

	unsigned int size = static_cast<unsigned int>(input.size() / rate);
	if (size % 2 != 0)
	{
		++size;
	}
	//�K���f�[�^
	std::vector<float>tmp(size);

	unsigned int index = 0;
	std::vector<float>right(input.size() / 2);
	std::for_each(right.begin(), right.end(), [&](float& i)->void {
		i = input[index];
		index += 2;
	});
	index = 1;
	std::vector<float>left(input.size() / 2);
	std::for_each(left.begin(), left.end(), [&](float& i)->void {
		i = input[index];
		index += 2;
	});

	float test = 0.0f;
	for (unsigned int i = 0; i < tmp.size(); i += 2)
	{
		tmp[i]     = right[test];
		tmp[i + 1] = left[test];

		test += rate;
	}

	return tmp;
}
