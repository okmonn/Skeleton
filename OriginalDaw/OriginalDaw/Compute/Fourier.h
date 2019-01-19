#pragma once
#include "Compute.h"
#include <vector>

// �t�[���G�ϊ��^�C�v
enum class FourierType {
	DFT,
	IDFT,
	FFT,
	IFFT
};

class Fourier : 
	public Compute
{
public:
	// �R���X�g���N�^
	Fourier(const std::wstring& fileName);
	// �f�X�g���N�^
	~Fourier();

	// ������
	void Init(const size_t& num);

	// �^�C�v�̕ύX
	void ChangeType(const FourierType& type) {
		this->type = type;
	}

	// ���s
	void Execution(std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// �f�[�^�̃R�s�[
	void Copy(const std::string& name, const std::vector<float>& data);
	void Copy(const std::string& name, const FourierType& type);

	// �f�[�^�̍X�V
	void UpData(const std::string& name, std::vector<float>& data);

	
	// �^�C�v
	FourierType type;
};
