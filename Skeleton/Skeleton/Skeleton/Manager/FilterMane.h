#pragma once
#include "Manager.h"

class Application;
class Sound;
class Mouse;

class FilterMane : 
	public Manager
{
public:
	// �R���X�g���N�^
	FilterMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~FilterMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// �J�b�g���g���̎擾
	float GetCut(void) const {
		return cut;
	}
	// �N�I���e�B�t�@�N�^�̎擾
	float GetQuality(void) const {
		return q;
	}
	// �N�I���e�B�t�@�N�^�̃Z�b�g
	void SetQuality(const float& q) {
		this->q = q;
	}

private:
	// �J�b�g���g��
	float cut;

	// �ߋ��̎��g��
	float oldCut;

	// �N�I���e�B�t�@�N�^
	float q;
};
