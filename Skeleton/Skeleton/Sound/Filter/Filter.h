#pragma once
#include <vector>
#include <cmath>

class Filter
{
public:
	// コンストラクタ
	Filter();
	// デストラクタ
	~Filter();

	// ローパスフィルタ
	void LowPass(const float& cutoff, const float& q = 1.0f / sqrt(2.0f));

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& out);

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
