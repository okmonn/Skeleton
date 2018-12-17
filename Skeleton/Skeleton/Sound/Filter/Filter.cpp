#include "Filter.h"
#include <iostream>

// 円周率
#define PI 3.14159265f

// コンストラクタ
Filter::Filter()
{
	memset(input, 0, sizeof(a));
	memset(out, 0, sizeof(b));
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	a[0] = 1.0f;
	b[0] = 1.0f;
}

// デストラクタ
Filter::~Filter()
{
}

// ローパスフィルタ
void Filter::LowPass(const float & cutoff, const float & q, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - cosf(omega)) / 2.0f;
	b[1] =  1.0f - cosf(omega);
	b[2] = (1.0f - cosf(omega)) / 2.0f;
}

// ハイパスフィルタ
void Filter::HighPass(const float & cutoff, const float & q, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] =  (1.0f + cosf(omega)) / 2.0f;
	b[1] = -(1.0f + cosf(omega));
	b[2] =  (1.0f + cosf(omega)) / 2.0f;
}

// バンドパスフィルタ
void Filter::BandPass(const float & cutoff, const float & bw, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) * sinhf(logf(2.0f) / 2.0f * bw * omega / sinf(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] =  alpha;
	b[1] =  0.0f;
	b[2] = -alpha;
}

// ノッチフィルタ
void Filter::Notch(const float & cutoff, const float & bw, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) * sinhf(logf(2.0f) / 2.0f * bw * omega / sinf(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * cosf(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f;
	b[1] = -2.0f * cosf(omega);
	b[2] =  1.0f;
}

// ローシェルフフィルタ
void Filter::Lowshelf(const float & cutoff, const float & gain, const float & q, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);
	float delta = powf(10.0f, gain / 40.0f);
	float beta  = sqrtf(delta) / q;

	a[0] = (delta + 1.0f) + (delta - 1.0f) * cosf(omega) + beta * sinf(omega);
	a[1] = -2.0f * ((delta - 1.0f) + (delta + 1.0f) * cosf(omega));
	a[2] = (delta + 1.0f) + (delta - 1.0f) * cosf(omega) - beta * sinf(omega);

	b[0] = delta * ((delta + 1.0f) - (delta - 1.0f) * cosf(omega) + beta * sinf(omega));
	b[1] = 2.0f * delta * ((delta - 1.0f) - (delta + 1.0f) * cosf(omega));
	b[2] = delta * ((delta + 1.0f) - (delta - 1.0f) * cosf(omega) - beta * sinf(omega));
}

// ハイシェルフフィルタ
void Filter::Highshelf(const float & cutoff, const float & gain, const float & q, const float & sample)
{
	float omega = 2.0f * PI * cutoff / sample;
	float alpha = sinf(omega) / (2.0f * q);
	float delta = powf(10.0f, gain / 40.0f);
	float beta  = sqrtf(delta) / q;

	a[0] = (delta + 1.0f) - (delta - 1.0f) * cosf(omega) + beta * sinf(omega);
	a[1] =  2.0f * ((delta - 1.0f) - (delta + 1.0f) * cosf(omega));
	a[2] = (delta + 1.0f) - (delta - 1.0f) * cosf(omega) - beta * sinf(omega);

	b[0] =  delta * ((delta + 1.0f) + (delta - 1.0f) * cosf(omega) + beta * sinf(omega));
	b[1] = -2.0f * delta * ((delta - 1.0f) + (delta + 1.0f) * cosf(omega));
	b[2] =  delta * ((delta + 1.0f) + (delta - 1.0f) * cosf(omega) - beta * sinf(omega));
}

// 実行
void Filter::Execution(const std::vector<float>& input, std::vector<float>& out)
{
	out.resize(input.size());
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		out[i] = b[0] / a[0] * input[i] + b[1] / a[0] * this->input[0] + b[2] / a[0] * this->input[1]
			- a[1] / a[0] * this->out[0] - a[2] / a[0] * this->out[1];

		this->input[1] = this->input[0];
		this->input[0] = input[i];

		this->out[1] = this->out[0];
		this->out[0] = out[i];
	}
}

// 実行
void Filter::Execution(std::vector<float>& data)
{
	float tmp = 0.0f;
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		tmp = data[i];

		data[i] = b[0] / a[0] * tmp + b[1] / a[0] * input[0] + b[2] / a[0] * input[1]
			- a[1] / a[0] * out[0] - a[2] / a[0] * out[1];

		input[1] = input[0];
		input[0] = tmp;

		out[1] = out[0];
		out[0] = data[i];
	}
}
