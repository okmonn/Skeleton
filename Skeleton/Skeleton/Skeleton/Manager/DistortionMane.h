#pragma once
#include "Manager.h"

class DistortionMane : 
	public Manager
{
public:
	// �R���X�g���N�^
	DistortionMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~DistortionMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// �������̎擾
	float GetAmp(void) const {
		return amplification;
	}

private:
	// ������
	float amplification;

	// �ߋ��̑�����
	float oldAmp;
};
