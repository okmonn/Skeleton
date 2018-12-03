#pragma once

class Filter
{
public:
	// �R���X�g���N�^
	Filter();
	// �f�X�g���N�^
	~Filter();

	// ���[�p�X
	void LowPass(const float& cutoff, const float& q, const int& sample = 44100);

	// ���͐M���Ƀt�B���^��K��
	float Process(const float& in);

private:
	// �t�B���^�W��
	float a0, a1, a2;
	float b0, b1, b2;

	// ���̓o�b�t�@
	float in1, in2;
	// �o�̓o�b�t�@
	float out1, out2;
};
