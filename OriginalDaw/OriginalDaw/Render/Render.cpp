#include "Render.h"
#include "../Device/Device.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// �R���X�g���N�^
Render::Render(std::weak_ptr<Swap>swap) : swap(swap), 
	heap(0)
{
	Create();
}

// �f�X�g���N�^
Render::~Render()
{
}

// �����_�[�̐���
void Render::Create(void)
{
	swap.lock()->Get()->GetDesc();
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE, );
}
