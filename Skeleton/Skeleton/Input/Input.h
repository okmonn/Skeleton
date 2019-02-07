#pragma once
#include "../InputInfo.h"

class Input
{
public:
	// デストラクタ
	~Input();

	// インスタンス変数の取得
	static Input& Get(void) {
		static Input instance;
		return instance;
	}

	// キーの確認
	bool CheckKey(const unsigned int& keyNo);

	// トリガーの確認
	bool CheckTrigger(const unsigned int& keyNo);

private:
	// コンストラクタ
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;


	// 現在のキー情報
	unsigned int now[256];

	// 過去のキー情報
	unsigned int old[256];
};
