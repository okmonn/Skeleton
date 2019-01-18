#pragma once
#include "Compute.h"
#include <vector>

class Fourier : 
	public Compute
{
public:
	// �R���X�g���N�^
	Fourier(std::wstring& fileName);
	// �f�X�g���N�^
	~Fourier();

	// ������
	void Init(const size_t& num);

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>imag);

private:
	// �f�[�^�̃R�s�[
	void Copy(const std::string& name, const std::vector<float>& data);

	// �f�[�^�̍X�V
	void UpData(const std::string& name, std::vector<float>& data);
};
