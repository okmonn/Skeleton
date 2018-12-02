#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <functional>

class SoundLoader
{
	struct Wave {
		//ファイルハンドル
		FILE* file;
		//チャンネル数
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
		//波形情報
		std::shared_ptr<std::unordered_map<int, std::vector<float>>>data;
	};

public:
	// デストラクタ
	~SoundLoader();

	// インスタンス変数の取得
	static SoundLoader& Get(void) {
		static SoundLoader instance;
		return instance;
	}

	// 読み込み
	int Load(const std::string& fileName);

	// チャンネル数の取得
	int GetChannel(const std::string& fileName) {
		return wave[fileName].channel;
	}
	// サンプリング周波数の取得
	int GetSample(const std::string& fileName) {
		return wave[fileName].sample;
	}
	// 量子化ビット数の取得
	int GetBit(const std::string& fileName) {
		return wave[fileName].bit;
	}
	// 波形データの取得
	std::shared_ptr<std::unordered_map<int, std::vector<float>>> GetWave(const std::string& fileName) {
		return wave[fileName].data;
	}
	// 読み込み完了フラグの取得
	bool GetFlag(const std::string& fileName) {
		return flag[fileName];
	}

private:
	// コンストラクタ
	SoundLoader();
	SoundLoader(const SoundLoader&) {
	}
	void operator=(const SoundLoader&) {
	}

	// 読み込みテーブルの生成
	void InitTbl(void);

	// 非同期読み込み
	void Stream(const std::string& fileName);


	// スレッド終了フラグ
	bool threadFlag;

	// 波形情報
	std::unordered_map<std::string, Wave>wave;

	// 波形読み込み完了フラグ
	std::unordered_map<std::string, bool>flag;

	// 読み込みスレッド
	std::vector<std::thread>th;

	// 読み込みテーブル
	std::unordered_map<int, std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
