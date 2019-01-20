#pragma once
#include "Compute.h"

class FFT :
	public Compute
{
public:
	// コンストラクタ
	FFT(const size_t& num);
	// デストラクタ
	~FFT();

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// 初期化
	void Init(const size_t& num);
};
