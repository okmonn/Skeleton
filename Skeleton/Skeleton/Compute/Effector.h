#pragma once
#include "Compute.h"
#include "../etc/tString.h"
#include <vector>

class Effector :
	public Compute
{
	// �p�����[�^
	struct Param {
		//�T���v�����O���g��
		int sample;
	};

public:
	// �R���X�g���N�^
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// �f�X�g���N�^
	~Effector();

	// ������
	void Init(const unsigned int& num);

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& out);

private:
};
