#pragma once

struct IXAudio2;

class Xaudio2
{
public:
	// デストラクタ
	~Xaudio2();

	// インスタンス変数の取得
	static Xaudio2& Get(void) {
		static Xaudio2 instance;
		return instance;
	}

	// Xaudio2の取得
	IXAudio2* GetAudio(void) const {
		return audio;
	}

private:
	// コンストラクタ
	Xaudio2();
	Xaudio2(const Xaudio2&) = delete;
	void operator=(const Xaudio2&) = delete;

	// COMの初期化
	long InitCom(void);

	// Xaudio2の生成
	long Create(void);


	// Xaudio2
	IXAudio2* audio;
};
