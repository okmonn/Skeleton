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
		//遅延器の数
		float delayDevNum;
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const unsigned int& num);

	// ローパスフィルタ
	void LowPass(const float& cutoff);

	// パラメータのセット
	void SetParam(void);

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& out);

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
