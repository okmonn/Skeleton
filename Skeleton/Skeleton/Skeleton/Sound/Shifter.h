#pragma once
#include <vector>

class Shifter
{
public:
	// �R���X�g���N�^
	Shifter();
	// �f�X�g���N�^
	~Shifter();

	// ������
	std::vector<float> FastForward(const std::vector<float>& input, const float& rate, const int& sample);

private:

};
