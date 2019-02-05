#pragma once
#include "Manager.h"

class NoiseMane :
	public Manager
{
public:
	// �R���X�g���N�^
	NoiseMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~NoiseMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// 臒l
	float threshold;

	// �O��臒l
	float oldThd;
};
