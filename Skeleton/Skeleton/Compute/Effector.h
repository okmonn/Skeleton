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
		//������
		float gain;
		//���ʃ��x��
		float volume;
		//�x����̐�
		float delayDevNum;
		//�t�B���^�K���t���O
		int filter;
	};

public:
	// �R���X�g���N�^
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// �f�X�g���N�^
	~Effector();

	// ������
	void Init(const unsigned int& num);

	// ���[�p�X�t�B���^
	void LowPass(const float& cutoff, const float& delta = 1000.0f);

	// �n�C�p�X�t�B���^
	void HightPass(const float& cutoff, const float& delta = 1000.0f);

	// ���s
	void Execution(const std::vector<float>& input, std::vector<float>& out);

	// �T���v�����O���g���̃Z�b�g
	void SetSample(const float& sample) {
		param.sample = sample;
	}
	// �������̃Z�b�g
	void SetGain(const float& gain) {
		param.gain = gain;
	}
	// �{�����[���̃Z�b�g
	void SetVolume(const float& volume) {
		param.volume = volume;
	}
	// �t�B���^�K���t���O�̃Z�b�g
	void SetFilter(const bool& flag) {
		param.filter = flag;
	}

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
