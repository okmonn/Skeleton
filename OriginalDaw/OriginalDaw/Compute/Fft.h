#pragma once
#include "Compute.h"

class FFT :
	public Compute
{
public:
	// �R���X�g���N�^
	FFT(const size_t& num);
	// �f�X�g���N�^
	~FFT();

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// ������
	void Init(const size_t& num);
};
