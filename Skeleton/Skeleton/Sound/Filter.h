#pragma once
#include "SndFunc.h"

class Filter
{
public:
	// コンストラクタ
	Filter();
	// デストラクタ
	~Filter();

	// 係数のリセット
	void Reset(void);

	// ローパスフィルタ
	// cutoffより低い周波数を通す
	void LowPass(const float& cutoff, const float& q, const snd::Info& info);

	// ハイパスフィルタ
	// cutoffより高い周波数を通す
	void HighPass(const float& cutoff, const float& q, const snd::Info& info);

	// バンドパスフィルタ
	// cutoffを中心にwb分の周波数を通す
	void BandPass(const float& cutoff, const float& bw, const snd::Info& info);

	// 実行
	std::vector<float> Execution(const std::vector<float>& input);

private:
	// 入力値の保持
	float input[2];

	// 出力値の保持
	float out[2];

	// フィルタ係数A
	float a[3];

	// フィルタ係数B
	float b[3];
};
