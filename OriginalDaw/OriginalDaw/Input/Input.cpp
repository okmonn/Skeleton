#include "Input.h"
#include "../Helper/Helper.h"

// コンストラクタ
Input::Input()
{
	memset(&key[0], 0, sizeof(key));
	memset(&old[0], 0, sizeof(old));
}

// デストラクタ
Input::~Input()
{
}

// キーの入力
bool Input::InputKey(const int & i)
{
	old[i] = key[i];
	key[i] = help::CheckKey(i);
	if (key[i])
	{
		return true;
	}

	return false;
}

// トリガーの入力
bool Input::Triger(const int & i)
{
	if (InputKey(i) == true && old[i] != key[i])
	{
		return true;
	}

	return false;
}
