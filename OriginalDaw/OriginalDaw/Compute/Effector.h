#pragma once
#include "Compute.h"
#include "../Sound/SndFunc.h"
#include <vector>

class Effector :
	public Compute
{
public:
	// コンストラクタ
	Effector(const std::wstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const size_t& num);

	// 実行
	void Execution(std::vector<float>& data);

private:
};
