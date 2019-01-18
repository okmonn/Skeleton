#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>
#include <cmath>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Filter;
class Effector;

class Sound
{
public:
	// コンストラクタ
	Sound();
	Sound(const std::string& fileName);
	Sound(const snd::Info& info);
	
	// デストラクタ
	~Sound();

	// 読み込み
	void Load(const std::string& fileName);

	// サウンド情報構造体のコピー
	void CopyInfo(const snd::Info& info);

	// ローパスフィルタ
	void LowPass(const float& cutoff, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

	// ハイパスフィルタ
	void HightPass(const float& cutoff, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

	// バンドパスフィルタ
	void BandPass(const float& cutoff, const float& sample, const float& bw = 1.0f);

	// 再生
	long Play(const bool& loop);

	// 停止
	long Stop(void);

	// ループ開始位置のセット
	void SetLoopPos(void);

	// ループ開始位置に移動
	void MoveLoopPos(void);

	// 現在のサウンド情報の取得
	std::vector<float> GetData(void) const {
		return data[index];
	}

private:
	//コピーコンストラクタ
	Sound(const Sound&) = delete;
	void operator=(const Sound&) = delete;

	// ソースボイスの生成
	long CreateVoice(void);

	// ファイルからの非同期処理
	void StreamFile(void);

	// コピーデータから非同期処理
	void StreamInfo(void);


	// ボイスコールバック
	std::unique_ptr<VoiceCallback>call;

	// フィルター
	std::unique_ptr<Filter>filter;

	// エフェクター
	std::unique_ptr<Effector>effe;

	// ボイスステータス
	std::unique_ptr<XAUDIO2_VOICE_STATE>state;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// ループフラグ
	bool loop;

	// スレッドフラグ
	bool flag;

	// バッファインデックス
	unsigned int index;

	// 読み込みインデックス
	unsigned int read;

	// ループ開始位置
	unsigned int loopPos;

	// 読み込みファイル名
	std::string name;

	// パラメータ
	snd::Param param;
	
	// コピー情報
	snd::Info copy;

	// 波形情報
	std::vector<std::vector<float>>data;

	// スレッド
	std::thread th;
};
