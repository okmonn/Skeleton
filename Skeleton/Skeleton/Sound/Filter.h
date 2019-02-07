#pragma once
#include "SndFunc.h"

class Filter
{
public:
	// �R���X�g���N�^
	Filter();
	// �f�X�g���N�^
	~Filter();

	// �W���̃��Z�b�g
	void Reset(void);

	// ���[�p�X�t�B���^
	// cutoff���Ⴂ���g����ʂ�
	void LowPass(const float& cutoff, const float& q, const snd::Info& info);

	// �n�C�p�X�t�B���^
	// cutoff��荂�����g����ʂ�
	void HighPass(const float& cutoff, const float& q, const snd::Info& info);

	// �o���h�p�X�t�B���^
	// cutoff�𒆐S��wb���̎��g����ʂ�
	void BandPass(const float& cutoff, const float& bw, const snd::Info& info);

	// ���s
	std::vector<float> Execution(const std::vector<float>& input);

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
