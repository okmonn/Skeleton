#include "Filter.h"
#include <math.h>

// コンストラクタ
Filter::Filter()
{
	a0 = 1.0f;
	a1 = 0.0f;
	a2 = 0.0f;

	b0 = 1.0f;
	b1 = 0.0f;
	b2 = 0.0f;

	in1 = 0.0f;
	in2 = 0.0f;

	out1 = 0.0f;
	out2 = 0.0f;
}

// デストラクタ
Filter::~Filter()
{
}

// ローパス
void Filter::LowPass(const float & cutoff, const float & q, const int & sample)
{
	//フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);

	// フィルタ係数を求める。
	a0 =  1.0f + alpha;
	a1 = -2.0f * cosf(omega);
	a2 =  1.0f - alpha;
	b0 = (1.0f - cosf(omega)) / 2.0f;
	b1 =  1.0f - cosf(omega);
	b2 = (1.0f - cosf(omega)) / 2.0f;
}

// 入力信号にフィルタを適応
float Filter::Process(const float & in)
{
	// 入力信号にフィルタを適用し、出力信号変数に保存。
	float out = (b0 / a0 * in) + (b1 / a0 * in1) + (b2 / a0 * in2) - (a1 / a0 * out1) - (a2 / a0 * out2);

	in2 = in1; // 2つ前の入力信号を更新
	in1 = in;  // 1つ前の入力信号を更新

	out2 = out1; // 2つ前の出力信号を更新
	out1 = out;  // 1つ前の出力信号を更新

	// 出力信号を返す
	return out;
}
