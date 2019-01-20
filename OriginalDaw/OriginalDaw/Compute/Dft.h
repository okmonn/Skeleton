#pragma once
#include "Compute.h"
#include <vector>

class DFT :
	public Compute
{
public:
	// �R���X�g���N�^
	DFT(const size_t& num);
	// �f�X�g���N�^
	~DFT();

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// ������
	void Init(const size_t& num);
};
