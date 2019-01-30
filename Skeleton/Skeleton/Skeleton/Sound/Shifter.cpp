#include "Shifter.h"
#include "../Useful/Useful.h"
#include <algorithm>

// コンストラクタ
Shifter::Shifter()
{
}

// デストラクタ
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

// 再生速度調節
std::vector<float> Shifter::TimeShift(const std::vector<float>& input, const float & rate)
{
	//サイズ調節
	unsigned int size = static_cast<unsigned int>(input.size() / rate);
	if (size % 2 != 0)
	{
		++size;
	}
	//適応データ
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

// ピッチ調節
std::vector<float> Shifter::PitchShift(const std::vector<float>& input, const float & pitch)
{
	return input;
}
