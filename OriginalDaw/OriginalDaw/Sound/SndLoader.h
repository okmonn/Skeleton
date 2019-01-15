#pragma once
#include "SndFunc.h"
#include <mutex>
#include <thread>
#include <functional>
#include <unordered_map>

class SndLoader
{
public:
	// デストラクタ
	~SndLoader();

	// インスタンス変数の取得
	static SndLoader& Get(void) {
		static SndLoader instance;
		return instance;
	}

	// 読み込み
	int Load(const std::string& fileName);

	// 削除
	void Delete(const std::string& fileName);

	// サウンド情報の取得
	snd::Snd GetSnd(const std::string& fileName) {
		return sound[fileName];
	}

private:
	// コンストラクタ
	SndLoader();
	SndLoader(const SndLoader&) = delete;
	void operator=(const SndLoader&) = delete;

	// 読み込みテーブルの作成
	void Create(void);

	// 非同期読み込み
	void Stream(const std::string& fileName);


	// スレッドフラグ
	bool flag;

	// ミューテックス
	std::mutex mtx;

	// サウンド情報
	std::unordered_map<std::string, snd::Snd>sound;

	// スレッド
	std::vector<std::thread>th;

	// 読み込みテーブル
	std::unordered_map<int, std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
