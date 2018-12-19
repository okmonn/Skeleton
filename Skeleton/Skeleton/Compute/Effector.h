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
		//変調深度
		float depth;
		//変調周波数
		float rate;
		//パンニング
		float left;
		//パンイング
		float right;
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
	void SetSample(const float& sample);

	// 増幅率のセット
	void SetGain(const float& gain);

	// ボリュームのセット
	void SetVolume(const float& volume);

	// 変調深度のセット
	void SetDepth(const float& depth);

	// 変調周波数のセット
	void SetRate(const float& rate);

	// パンニング比率のセット
	void SetPan(const float& left, const float& right);

private:
	// パラメータ
	Param param;
};
