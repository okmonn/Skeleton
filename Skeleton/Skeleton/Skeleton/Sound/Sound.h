#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Effector;
class Shifter;
class Filter;

class Sound
{
public:
	// コンストラクタ
	Sound();
	// デストラクタ
	~Sound();

	// 読み込み
	int Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

	// ループ位置のセット
	void SetLoopPos(void);

	// ループ位置に移動
	void MoveLoopPos(void);

private:
	// ソースボイスの生成
	long CreateVoice(void);

	// 非同期処理
	void Stream(void);


	// コールバック
	std::unique_ptr<VoiceCallback>back;

	// エフェクター
	std::unique_ptr<Effector>effe;

	// シフター
	std::unique_ptr<Shifter>shifter;

	// フィルター
	std::unique_ptr<Filter>filter;

	// ボイスステータス
	std::unique_ptr<XAUDIO2_VOICE_STATE>st;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// インデックス
	unsigned int index;

	// 読み込みインデックス
	unsigned int read;

	// ループポイント
	unsigned int loopPos;

	// ループフラグ
	bool loop;

	// スレッドフラグ
	bool threadFlag;

	// 読み込みファイル名
	std::string name;

	// サウンド情報
	snd::Info info;

	// パラメータ
	snd::Param param;

	// サウンドバッファ
	std::vector<std::vector<float>>data;

	// スレッド
	std::thread th;
};
