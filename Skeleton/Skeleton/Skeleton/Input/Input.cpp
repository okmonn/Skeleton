#include "Input.h"
#include "../Useful/Useful.h"

// コンストラクタ
Input::Input()
{
	memset(&now[0], 0, sizeof(now));
	memset(&old[0], 0, sizeof(old));
}

// デストラクタ
Input::~Input()
{
}

// キーの確認
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

// トリガーの確認
bool Input::CheckTrigger(const unsigned int & keyNo)
{
	if (CheckKey(keyNo) == true && old[keyNo] != now[keyNo])
	{
		return true;
	}

	return false;
}
