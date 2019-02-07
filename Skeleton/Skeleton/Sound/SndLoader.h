#pragma once
#include "SndFunc.h"
#include <memory>
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

	// サウンド情報の取得
	snd::Info GetInfo(const std::string& fileName);

	// サウンドデータの取得
	std::shared_ptr<std::vector<float>> GetData(const std::string& fileName);

	// 削除
	void Delete(const std::string& fileName);

private:
	// コンストラクタ
	SndLoader();
	SndLoader(const SndLoader&) = delete;
	void operator=(const SndLoader&) = delete;

	// 初期化
	void Init(void);


	// サウンド情報
	std::unordered_map<std::string, snd::Info>info;

	// サウンドデータ
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// 関数ポインタ
	std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>load;
};
