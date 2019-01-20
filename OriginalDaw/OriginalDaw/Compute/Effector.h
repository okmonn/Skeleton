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

	// ������
	void Init(const size_t& num);

	// ���s
	void Execution(std::vector<float>& data);

private:
};
