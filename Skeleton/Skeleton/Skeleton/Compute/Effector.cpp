#include "Effector.h"

// リソース数
#define RSC_NUM 3

// コンストラクタ
Effector::Effector(const size_t & num)
{
	Init(num);
}

// デストラクタ
Effector::~Effector()
{
	End();
}

// 初期化
void Effector::Init(const size_t & num)
{
	CreateHeap(RSC_NUM);

	CBV("param",  sizeof(float));
	UAV("input",  sizeof(float), num);
	UAV("output", sizeof(float), num);
}

// 実行
void Effector::Execution()
{
}
