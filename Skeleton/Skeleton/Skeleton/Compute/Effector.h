#pragma once
#include "Compute.h"

class Effector :
	public Compute
{
public:
	// �R���X�g���N�^
	Effector(const size_t& num);
	// �f�X�g���N�^
	~Effector();

	// ���s
	void Execution();

private:
	// ������
	void Init(const size_t& num);
};
