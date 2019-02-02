#pragma once
#include "SndFunc.h"

class Delay
{
public:
	// �R���X�g���N�^
	Delay();
	// �f�X�g���N�^
	~Delay();

	// �p�����[�^�̃Z�b�g
	void SetParam(const snd::DelayParam& param);

	// ���s
	std::vector<float> Execution(const std::vector<float>& input, const snd::Info& info, const unsigned int offset = 0);

	// �N���A
	void Clear(void);

private:
	// �p�����[�^
	snd::DelayParam param;

	// �ߋ��̃f�[�^
	std::vector<std::vector<float>>old;
};
