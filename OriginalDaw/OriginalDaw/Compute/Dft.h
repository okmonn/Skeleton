#pragma once
#include "Compute.h"
#include <vector>

class DFT :
	public Compute
{
public:
	// コンストラクタ
	DFT(const size_t& num);
	// デストラクタ
	~DFT();

	// 実行(float版)
	// 実行速度は速いが精度は落ちる
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);
	// 実行(double版)
	// 実行速度は速いが精度は落ちる
	void Execution(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

private:
	// 初期化
	void Init(const size_t& num);
};
