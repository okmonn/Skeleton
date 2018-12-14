#pragma once
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>

class SoundLoader
{
	//サウンド情報
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
		std::vector<std::vector<float>>data;
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
	// 波形情報の取得
	std::vector<std::vector<float>>& GetWave(const std::string& fileName) {
		return wave[fileName].data;
	}

private:
	// コンストラクタ
	SoundLoader();
	SoundLoader(const SoundLoader&) = delete;
	void operator=(const SoundLoader&) = delete;

	// 読み込みテーブルの生成
	void InitLoadTbl(void);

	// 非同期読み込み
	void Stream(const std::string& fileName);


	// スレッド終了フラグ
	bool threadFlag;

	// スレッド
	std::vector<std::thread>th;

	// サウンド情報
	std::unordered_map<std::string, Wave>wave;

	// 読み込みテーブル
	std::unordered_map<int, std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
