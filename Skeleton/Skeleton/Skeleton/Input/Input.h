#pragma once
#include "../InputInfo.h"

class Input
{
public:
	// �f�X�g���N�^
	~Input();

	// �C���X�^���X�ϐ��̎擾
	static Input& Get(void) {
		static Input instance;
		return instance;
	}

	// �L�[�̊m�F
	bool CheckKey(const unsigned int& keyNo);

	// �g���K�[�̊m�F
	bool CheckTrigger(const unsigned int& keyNo);

private:
	// �R���X�g���N�^
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;


	// ���݂̃L�[���
	unsigned int now[256];

	// �ߋ��̃L�[���
	unsigned int old[256];
};
