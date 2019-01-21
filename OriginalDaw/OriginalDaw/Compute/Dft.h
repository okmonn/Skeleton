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

	// ���s(float��)
	// ���s���x�͑��������x�͗�����
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);
	// ���s(double��)
	// ���s���x�͑��������x�͗�����
	void Execution(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

private:
	// ������
	void Init(const size_t& num);
};
