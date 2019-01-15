#pragma once
#include <cmath>
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
	void LowPass(const float& cutoff, const float& sample, const float& q);

	// ハイパスフィルタ
	// cutoffより高い周波数を通す
	void HighPass(const float& cutoff, const float& sample, const float& q);

	// バンドパスフィルタ
	// cutoffを中心にwb分の周波数を通す
	void BandPass(const float& cutoff, const float& sample, const float& bw = 1.0f);

	// ノッチフィルタ
	// cutoffを中心にwb分の周波数以外を通す
	void Notch(const float& cutoff, const float& sample, const float& bw = 1.0f);

	// ローシェルフフィルタ
	// cutoffより低い周波数をgain分だけ増幅する
	void Lowshelf(const float& cutoff, const float& gain, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

	// ハイシェルフフィルタ
	// cutoffより高い周波数をgain分だけ増幅する
	void Highshelf(const float& cutoff, const float& gain, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

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
