#pragma once
#include "Manager.h"

class CompMane :
	public Manager
{
public:
	// �R���X�g���N�^
	CompMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~CompMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// 臒l
	float threshold;

	// �O��臒l
	float oldThd;

	// ���k�䗦
	float ratio;

	// �O�̈��k�䗦
	float oldRatio;
};
