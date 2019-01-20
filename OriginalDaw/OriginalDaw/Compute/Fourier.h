#pragma once
#include "Compute.h"
#include <vector>

// �t�[���G�ϊ��^�C�v
enum class FourierType {
	//���U�t�[���G�ϊ�
	DFT,
	//�����t�[���G�ϊ�
	FFT
};

class Fourier : 
	public Compute
{
	// �p�����[�^
	struct Param {
		//�^�C�v
		FourierType type;
		//��
		unsigned int stage;
	};

public:
	// �R���X�g���N�^
	Fourier(const std::wstring& fileName);
	// �f�X�g���N�^
	~Fourier();

	// ������
	void Init(const size_t& num);

	// �^�C�v�̕ύX
	void SetParam(const FourierType& type, const unsigned int& stage = 0) {
		param = { type, stage };
	}

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// �f�[�^�̃R�s�[
	void Copy(const std::string& name, const std::vector<float>& data);
	void Copy(const std::string& name, const Param& param);

	// �f�[�^�̍X�V
	void UpData(const std::string& name, std::vector<float>& data);

	
	// �p�����[�^
	Param param;
};
