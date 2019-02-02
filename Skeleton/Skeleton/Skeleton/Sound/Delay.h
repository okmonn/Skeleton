#pragma once
#include "SndFunc.h"

class Delay
{
public:
	// コンストラクタ
	Delay();
	// デストラクタ
	~Delay();

	// パラメータのセット
	void SetParam(const snd::DelayParam& param);

	// 実行
	std::vector<float> Execution(const std::vector<float>& input, const snd::Info& info, const unsigned int offset = 0);

	// クリア
	void Clear(void);

private:
	// パラメータ
	snd::DelayParam param;

	// 過去のデータ
	std::vector<std::vector<float>>old;
};
