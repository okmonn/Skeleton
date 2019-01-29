#pragma once
#include <vector>

class Filter
{
public:
	// �R���X�g���N�^
	Filter();
	// �f�X�g���N�^
	~Filter();

	// ���[�p�X�t�B���^
	// cutoff���Ⴂ���g����ʂ�
	void LowPass(const float& cutoff, const float& q, const int& sample);

	// �n�C�p�X�t�B���^
	// cutoff��荂�����g����ʂ�
	void HighPass(const float& cutoff, const float& q, const int& sample);

	// �o���h�p�X�t�B���^
	// cutoff�𒆐S��wb���̎��g����ʂ�
	void BandPass(const float& cutoff, const float& bw, const int& sample);

	// ���s
	void Execution(std::vector<float>& data);

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
