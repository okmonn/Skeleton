#include "Filter.h"
#include <algorithm>

// 円周率
#define PI 3.14159265f

// コンストラクタ
Filter::Filter()
{
	memset(input, 0, sizeof(input));
	memset(out,   0, sizeof(out));
	memset(a,     0, sizeof(a));
	memset(b,     0, sizeof(b));
	a[0] = 1.0f;
	b[0] = 1.0f;
}

// デストラクタ
Filter::~Filter()
{
}

// ローパスフィルタ
void Filter::LowPass(const float & cutoff, const float & q, const snd::Info & info)
{
	float omega = 2.0f * PI * cutoff / info.sample;
	float alpha = std::sin(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - std::cos(omega)) / 2.0f;
	b[1] =  1.0f - std::cos(omega);
	b[2] = (1.0f - std::cos(omega)) / 2.0f;
}

// ハイパスフィルタ
void Filter::HighPass(const float & cutoff, const float & q, const snd::Info & info)
{
	float omega = 2.0f * PI * cutoff / info.sample;
	float alpha = std::sinf(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] =  (1.0f + std::cosf(omega)) / 2.0f;
	b[1] = -(1.0f + std::cosf(omega));
	b[2] =  (1.0f + std::cosf(omega)) / 2.0f;
}

// バンドパスフィルタ
void Filter::BandPass(const float & cutoff, const float & bw, const snd::Info & info)
{
	float omega = 2.0f * PI * cutoff / info.sample;
	float alpha = std::sinf(omega) * std::sinhf(logf(2.0f) / 2.0f * bw * omega / std::sinf(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] =  alpha;
	b[1] =  0.0f;
	b[2] = -alpha;
}

// 実行
std::vector<float> Filter::Execution(const std::vector<float>& input)
{
	std::vector<float>adap(input.size());
	unsigned int index = 0;
	std::for_each(adap.begin(), adap.end(), [&](float& i)->void {
		i = b[0] / a[0] * input[index]
		  + b[1] / a[0] * this->input[0]
		  + b[2] / a[0] * this->input[1]
		  - a[1] / a[0] * out[0]
		  - a[2] / a[0] * out[1];

		this->input[1] = this->input[0];
		this->input[0] = input[index];

		out[1] = out[0];
		out[0] = adap[index];

		++index;
	});

	return adap;
}
