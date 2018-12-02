#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// デストラクタ
	~XAudio2();

	// インスタンス変数の取得
	static XAudio2& Get(void) {
		static XAudio2 instance;
		return instance;
	}

	// オーディオの取得
	IXAudio2* GetAudio(void) const {
		return audio;
	}
	// マスタリングの取得
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// コンストラクタ
	XAudio2();
	XAudio2(const XAudio2&) {
	}
	void operator=(const XAudio2&) {
	}

	// COMの初期化
	long InitCom(void);

	// オーディオの生成
	long CreateAudio(void);

	// マスタリングの生成
	long CreateMastering(void);

	// 初期化
	void Init(void);


	// オーディオ
	IXAudio2* audio;

	// マスタリング
	IXAudio2MasteringVoice* mastering;
};
