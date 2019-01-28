#pragma once
#include "Compute.h"

class Effector :
	public Compute
{
public:
	// コンストラクタ
	Effector(const size_t& num);
	// デストラクタ
	~Effector();

	// 実行
	void Execution();

private:
	// 初期化
	void Init(const size_t& num);
};
