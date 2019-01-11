#include "Render.h"
#include "../Device/Device.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"

// コンストラクタ
Render::Render(std::weak_ptr<Swap>swap) : swap(swap), 
	heap(0)
{
}

// デストラクタ
Render::~Render()
{
}

// レンダーの生成
void Render::Create(void)
{
}
