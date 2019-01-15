#pragma once
#include "Compute.h"

class Effector :
	public Compute
{
public:
	// コンストラクタ
	Effector(const std::wstring& fileName);
	// デストラクタ
	~Effector();

	// 実行
	void Execution(void);

private:

};
