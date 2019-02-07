#include "Input.h"
#include "../Useful/Useful.h"

// �R���X�g���N�^
Input::Input()
{
	memset(&now[0], 0, sizeof(now));
	memset(&old[0], 0, sizeof(old));
}

// �f�X�g���N�^
Input::~Input()
{
}

// �L�[�̊m�F
bool Input::CheckKey(const unsigned int & keyNo)
{
	old[keyNo] = now[keyNo];
	now[keyNo] = use::CheckKey(keyNo);
	if (now[keyNo])
	{
		return true;
	}

	return false;
}

// �g���K�[�̊m�F
bool Input::CheckTrigger(const unsigned int & keyNo)
{
	if (CheckKey(keyNo) == true && old[keyNo] != now[keyNo])
	{
		return true;
	}

	return false;
}
