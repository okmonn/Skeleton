#include "Effector.h"

// ���\�[�X��
#define RSC_NUM 3

// �R���X�g���N�^
Effector::Effector(const size_t & num)
{
	Init(num);
}

// �f�X�g���N�^
Effector::~Effector()
{
	End();
}

// ������
void Effector::Init(const size_t & num)
{
	CreateHeap(RSC_NUM);

	CBV("param",  sizeof(float));
	UAV("input",  sizeof(float), num);
	UAV("output", sizeof(float), num);
}

// ���s
void Effector::Execution()
{
}
