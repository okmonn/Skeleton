#include "ShadowMap.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"

// コンストラクタ
ShadowMap::ShadowMap(std::weak_ptr<Device> dev, const unsigned int & width, const unsigned int & height) : 
	descMane(DescriptorMane::Get()), dev(dev)
{
}

// デストラクタ
ShadowMap::~ShadowMap()
{
}
