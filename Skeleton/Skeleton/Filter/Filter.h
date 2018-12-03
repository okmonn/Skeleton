#pragma once

class Filter
{
public:
	// コンストラクタ
	Filter();
	// デストラクタ
	~Filter();

	// ローパス
	void LowPass(const float& cutoff, const float& q, const int& sample = 44100);

	// 入力信号にフィルタを適応
	float Process(const float& in);

private:
	// フィルタ係数
	float a0, a1, a2;
	float b0, b1, b2;

	// 入力バッファ
	float in1, in2;
	// 出力バッファ
	float out1, out2;
};
