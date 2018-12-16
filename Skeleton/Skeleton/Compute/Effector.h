#pragma once
#include "Compute.h"
#include "../etc/tString.h"
#include <vector>

class Effector :
	public Compute
{
	// パラメータ
	struct Param {
		//サンプリング周波数
		float sample;
		//増幅率
		float gain;
		//音量レベル
		float volume;
		//遅延器の数
		float delayDevNum;
		//フィルタ適応フラグ
		int filter;
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const unsigned int& num);

	// ローパスフィルタ
	void LowPass(const float& cutoff, const float& delta = 1000.0f);

	// ハイパスフィルタ
	void HightPass(const float& cutoff, const float& delta = 1000.0f);

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& out);

	// サンプリング周波数のセット
	void SetSample(const float& sample) {
		param.sample = sample;
	}
	// 増幅率のセット
	void SetGain(const float& gain) {
		param.gain = gain;
	}
	// ボリュームのセット
	void SetVolume(const float& volume) {
		param.volume = volume;
	}
	// フィルタ適応フラグのセット
	void SetFilter(const bool& flag) {
		param.filter = flag;
	}

private:
	// ハニング窓関数
	float Haninng(const int& i, const int& size);

	// シンク関数
	float Sinc(const float& i);


	// パラメータ
	Param param;

	// デジタルフィルタの係数
	std::vector<float>coe;
};
