#include "ShadowMap.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"

// �R���X�g���N�^
ShadowMap::ShadowMap(std::weak_ptr<Device> dev, const unsigned int & width, const unsigned int & height) : 
	descMane(DescriptorMane::Get()), dev(dev)
{
}

// �f�X�g���N�^
ShadowMap::~ShadowMap()
{
}
