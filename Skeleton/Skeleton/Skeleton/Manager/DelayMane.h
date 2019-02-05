#pragma once
#include "Manager.h"

class DelayMane :
	public Manager
{
public:
	// �R���X�g���N�^
	DelayMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~DelayMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// ������
	float attenuation;

	// �O�̌�����
	float oldAtten;

	// �x������
	float delayTime;

	// �O�̒x������
	float oldTime;

	// ���[�v��
	float loop;

	// �O�̃��[�v��
	float oldLoop;
};
