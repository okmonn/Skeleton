#include "Filter.h"
#include <math.h>

// �R���X�g���N�^
Filter::Filter()
{
	a0 = 1.0f;
	a1 = 0.0f;
	a2 = 0.0f;

	b0 = 1.0f;
	b1 = 0.0f;
	b2 = 0.0f;

	in1 = 0.0f;
	in2 = 0.0f;

	out1 = 0.0f;
	out2 = 0.0f;
}

// �f�X�g���N�^
Filter::~Filter()
{
}

// ���[�p�X
void Filter::LowPass(const float & cutoff, const float & q, const int & sample)
{
	//�t�B���^�W���v�Z�Ŏg�p���钆�Ԓl�����߂�B
	float omega = 2.0f * 3.14159265f *  cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);

	// �t�B���^�W�������߂�B
	a0 =  1.0f + alpha;
	a1 = -2.0f * cosf(omega);
	a2 =  1.0f - alpha;
	b0 = (1.0f - cosf(omega)) / 2.0f;
	b1 =  1.0f - cosf(omega);
	b2 = (1.0f - cosf(omega)) / 2.0f;
}

// ���͐M���Ƀt�B���^��K��
float Filter::Process(const float & in)
{
	// ���͐M���Ƀt�B���^��K�p���A�o�͐M���ϐ��ɕۑ��B
	float out = (b0 / a0 * in) + (b1 / a0 * in1) + (b2 / a0 * in2) - (a1 / a0 * out1) - (a2 / a0 * out2);

	in2 = in1; // 2�O�̓��͐M�����X�V
	in1 = in;  // 1�O�̓��͐M�����X�V

	out2 = out1; // 2�O�̏o�͐M�����X�V
	out1 = out;  // 1�O�̏o�͐M�����X�V

	// �o�͐M����Ԃ�
	return out;
}
