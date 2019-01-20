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

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// 初期化
	void Init(const size_t& num);
};
