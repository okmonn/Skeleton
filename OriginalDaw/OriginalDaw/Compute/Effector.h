#pragma once
#include "Compute.h"
#include "../Sound/SndFunc.h"
#include <vector>

class Effector :
	public Compute
{
public:
	// �R���X�g���N�^
	Effector(const std::wstring& fileName);
	// �f�X�g���N�^
	~Effector();

	// �f�[�^�̃R�s�[
	void Copy(const std::string& name, const snd::Param& param);

	// ���s
	void Execution(std::vector<float>& data);

private:
	// ������
	void Init(void);

	// �f�[�^�̃R�s�[
	void Copy(const std::string& name, const std::vector<float>& data);

	// �f�[�^�̍X�V
	void UpData(const std::string& name, std::vector<float>& data);
};
