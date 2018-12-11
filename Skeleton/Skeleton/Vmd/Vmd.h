#pragma once
#include "VmdData.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Vmd
{
public:
	// デストラクタ
	~Vmd();

	// インスタンス変数の取得
	static Vmd& Get(void) {
		static Vmd instance;
		return instance;
	}

	// 読み込み
	int Load(const std::string& fileName);

	// モーションデータの取得
	std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>> GetMotion(const std::string& fileName) {
		return motion[fileName];
	}

	// アニメーションの時間の取得
	float GetAnimTime(const std::string& fileName) {
		return animTime[fileName];
	}

private:
	// コンストラクタ
	Vmd();
	Vmd(const Vmd&) = delete;
	void operator=(const Vmd&) = delete;

	// モーション情報
	std::unordered_map<std::string, std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>>>motion;

	// アニメーションの時間
	std::unordered_map<std::string, float>animTime;
};
