#pragma once
#include "SndFunc.h"

class Shifter
{
public:
	// �R���X�g���N�^
	Shifter();
	// �f�X�g���N�^
	~Shifter();

	// �Đ����x����
	std::vector<float> TimeShift(const std::vector<float>& input, const float& rate);

	// �s�b�`����
	std::vector<float> PitchShift(const std::vector<float>& input, const float& pitch);

private:

};
