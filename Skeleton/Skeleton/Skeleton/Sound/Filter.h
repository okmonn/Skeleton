#pragma once
#include <vector>

class Filter
{
public:
	// コンストラクタ
	Filter();
	// デストラクタ
	~Filter();

	// ローパスフィルタ
	// cutoffより低い周波数を通す
	void LowPass(const float& cutoff, const float& q, const int& sample);

	// ハイパスフィルタ
	// cutoffより高い周波数を通す
	void HighPass(const float& cutoff, const float& q, const int& sample);

	// バンドパスフィルタ
	// cutoffを中心にwb分の周波数を通す
	void BandPass(const float& cutoff, const float& bw, const int& sample);

	// 実行
	void Execution(std::vector<float>& data);

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
