#include "Render.h"
#include "../Device/Device.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Render::Render(std::weak_ptr<Swap>swap) : swap(swap), 
	heap(0)
{
}

// �f�X�g���N�^
Render::~Render()
{
}

// �����_�[�̐���
void Render::Create(void)
{
}
