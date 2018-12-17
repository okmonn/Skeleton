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
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const unsigned int& num);

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

private:
	// パラメータ
	Param param;
};
