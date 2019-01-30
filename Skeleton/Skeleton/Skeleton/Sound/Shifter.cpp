#include "Shifter.h"
#include "../Useful/Useful.h"
#include <algorithm>

// �R���X�g���N�^
Shifter::Shifter()
{
}

// �f�X�g���N�^
Shifter::~Shifter()
{
}

std::vector<float> test(const std::vector<float>& input, const float& rate)
{
	unsigned int no = 0;
	std::vector<float>right(input.size() / 2);
	std::for_each(right.begin(), right.end(), [&](float& i)->void {
		i = input[no];
		no += 2;
	});

	no = 1;
	std::vector<float>left(input.size() / 2);
	std::for_each(left.begin(), left.end(), [&](float& i)->void {
		i = input[no];
		no += 2;
	});

	unsigned int size = static_cast<unsigned int>(input.size() / rate);
	if (size % 2 != 0)
	{
		++size;
	}
	std::vector<float>tmp(size);
}

// �Đ����x����
std::vector<float> Shifter::TimeShift(const std::vector<float>& input, const float & rate)
{
	//�T�C�Y����
	unsigned int size = static_cast<unsigned int>(input.size() / rate);
	if (size % 2 != 0)
	{
		++size;
	}
	//�K���f�[�^
	std::vector<float>tmp(size);

	unsigned int no = 0;
	std::vector<float>right(input.size() / 2);
	std::for_each(right.begin(), right.end(), [&](float& i)->void {
		i = input[no];
		no += 2;
	});
	no = 1;
	std::vector<float>left(input.size() / 2);
	std::for_each(left.begin(), left.end(), [&](float& i)->void {
		i = input[no];
		no += 2;
	});

	float index = 0.0f;
	for (unsigned int i = 0; i < tmp.size(); i += 2)
	{
		tmp[i]     = right[static_cast<unsigned int>(index)];
		tmp[i + 1] = left[static_cast<unsigned int>(index)];

		index += rate;
	}

	return tmp;
}

// �s�b�`����
std::vector<float> Shifter::PitchShift(const std::vector<float>& input, const float & pitch)
{
	return input;
}
