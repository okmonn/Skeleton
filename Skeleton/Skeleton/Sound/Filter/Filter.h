#pragma once
#include <vector>
#include <cmath>

class Filter
{
public:
	// �R���X�g���N�^
	Filter();
	// �f�X�g���N�^
	~Filter();

	// ���[�p�X�t�B���^
	void LowPass(const float& cutoff, const float& q = 1.0f / sqrt(2.0f));

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& out);

private:
	// ���͒l�̕ێ�
	float input[2];

	// �o�͒l�̕ێ�
	float out[2];

	// �t�B���^�W��A
	float a[3];

	// �t�B���^�W��B
	float b[3];
};
