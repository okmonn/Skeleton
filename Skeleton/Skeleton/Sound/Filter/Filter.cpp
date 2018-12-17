#include "Filter.h"
#include <iostream>

// 円周率
#define PI 3.14159265f

// コンストラクタ
Filter::Filter()
{
	memset(&input[0], 0, sizeof(a));
	memset(&out[0], 0, sizeof(b));
	memset(&a[0], 0, sizeof(a));
	memset(&b[0], 0, sizeof(b));
	a[0] = 1.0f;
	b[0] = 1.0f;
}

// デストラクタ
Filter::~Filter()
{
}

// ローパスフィルタ
void Filter::LowPass(const float & cutoff, const float & q)
{
}

// 実行
void Filter::Execution(const std::vector<float>& input, std::vector<float>& out)
{
	out.resize(input.size());
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		out[i] = b[0] / a[0] * input[i] + b[1] / a[0] * this->input[0] + b[2] / a[0] * this->input[1]
			- a[1] / a[0] * this->out[0] - a[2] / a[0] * this->out[1];
	}
}
