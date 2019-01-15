#pragma once

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

	// キーの入力
	bool InputKey(const int& i);

	// トリガーの入力
	bool Triger(const int& i);

private:
	// コンストラクタ
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;


	// 現在のキー情報
	int key[256];

	// 過去のキー情報
	int old[256];
};
