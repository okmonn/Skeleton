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
		float sample;
		//�x����̐�
		float delayDevNum;
	};

public:
	// �R���X�g���N�^
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// �f�X�g���N�^
	~Effector();

	// ������
	void Init(const unsigned int& num);

	// ���[�p�X�t�B���^
	void LowPass(const float& cutoff);

	// �p�����[�^�̃Z�b�g
	void SetParam(void);

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& out);

private:
	// �n�j���O���֐�
	float Haninng(const int& i, const int& size);
	// �V���N�֐�
	float Sinc(const float& i);


	// �p�����[�^
	Param param;

	// �f�W�^���t�B���^�̌W��
	std::vector<float>coe;
};
