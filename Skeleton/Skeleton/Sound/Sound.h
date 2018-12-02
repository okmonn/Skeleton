#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>

struct IXAudio2SourceVoice;

class XAudio2;
class VoiceCallback;
class SoundLoader;
class Effector;

class Sound
{
public:
	// コンストラクタ
	Sound();
	Sound(std::weak_ptr<Effector>effe);
	// デストラクタ
	~Sound();

	// 読み込み
	void Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop);

	// 停止
	long Stop(void);

	// コールバックハンドルの取得
	void* GetHandle(void) const;

	// 再生終了フラグの取得
	bool GetEnd(void) const {
		return end;
	}
	// 読み取り配列番号の取得
	unsigned int GetRead(void) const {
		return read;
	}
	// 配列番号の取得
	unsigned int GetIndex(void) const {
		return index;
	}
	// 波形データの取得
	std::vector<float> GetWave(void) {
		return wave[index];
	}
	std::vector<float> GetWave(const unsigned int& i) {
		return wave[i];
	}

private:
	Sound(const Sound&);
	void operator=(const Sound&) {
	}


	// ソースボイスの生成
	long CreateVoice(const std::string& filName);

	// 非同期処理
	void Stream(void);


	// オーディオ
	XAudio2& audio;

	// ローダー
	SoundLoader& loader;

	// エフェクター
	std::weak_ptr<Effector>effe;
	
	// コールバック
	std::unique_ptr<VoiceCallback>call;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// ループフラグ
	bool loop;

	// 再生終了フラグ
	bool end;

	// スレッドフラグ
	bool threadFlag;

	// 読み取り配列番号
	unsigned int read;

	// 配列番号
	unsigned int index;

	// 読み込みファイル名
	std::string name;

	// 非同期スレッド
	std::thread th;

	// 波形データ
	std::vector<std::vector<float>>wave;
};
