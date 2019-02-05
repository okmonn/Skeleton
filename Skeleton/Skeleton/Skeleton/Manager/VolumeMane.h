#pragma once
#include "Manager.h"

class VolumeMane :
	public Manager
{
public:
	// �R���X�g���N�^
	VolumeMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~VolumeMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// ����
	float volume;

	// �O�̉���
	float oldVolume;
};
