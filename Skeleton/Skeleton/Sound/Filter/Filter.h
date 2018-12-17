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
	// cutoffより低い周波数を通す
	void LowPass(const float& cutoff, const float& q = 1.0f / sqrtf(2.0f), const float& sample = 44100.0f);

	// ハイパスフィルタ
	// cutoffより高い周波数を通す
	void HighPass(const float& cutoff, const float& q = 1.0f / sqrtf(2.0f), const float& sample = 44100.0f);

	// バンドパスフィルタ
	// cutoffを中心にwb分の周波数を通す
	void BandPass(const float& cutoff, const float& bw = 1.0f, const float& sample = 44100.0f);

	// ノッチフィルタ
	// cutoffを中心にwb分の周波数以外を通す
	void Notch(const float& cutoff, const float& bw = 1.0f, const float& sample = 44100.0f);

	// ローシェルフフィルタ
	// cutoffより低い周波数をgain分だけ増幅する
	void Lowshelf(const float& cutoff, const float& gain, const float& q = 1.0f / sqrtf(2.0f), const float& sample = 44100.0f);

	// ハイシェルフフィルタ
	// cutoffより高い周波数をgain分だけ増幅する
	void Highshelf(const float& cutoff, const float& gain, const float& q = 1.0f / sqrtf(2.0f), const float& sample = 44100.0f);

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& out);
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
