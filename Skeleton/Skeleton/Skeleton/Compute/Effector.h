#pragma once
#include "Compute.h"
#include "../Sound/SndFunc.h"

class Effector :
	public Compute
{
public:
	// �R���X�g���N�^
	Effector(const std::string& fileName);
	// �f�X�g���N�^
	~Effector();

	// ������
	void Init(const size_t& num);

	// ���s
	std::vector<float> Execution(const std::vector<float>& input);

	// �p�����[�^�̃R�s�[
	void Copy(const snd::Param& param) {
		this->param = param;
	}

private:
	// �R�s�[
	void Copy(const std::string& name, const snd::Param& param);
	void Copy(const std::string& name, const std::vector<float>& data);

	// �X�V
	void UpData(const std::string& name, std::vector<float>& data);


	// �T�E���h�p�����[�^
	snd::Param param;
};
