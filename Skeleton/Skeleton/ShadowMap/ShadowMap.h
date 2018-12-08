#pragma once
#include <memory>

class DescriptorMane;
class Device;
class List;

class ShadowMap
{
public:
	// �R���X�g���N�^�@
	ShadowMap(std::weak_ptr<Device>dev, const unsigned int& width, const unsigned int& height);
	// �f�X�g���N�^
	~ShadowMap();

	// �[�x�q�[�vID�̎擾
	int& GetDepthHeap(void) {
		return dHeap;
	}

	// �V�F�[�_�[�q�[�vID�̎擾
	int& GetShaderHeap(void) {
		return sHeap;
	}

private:
	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �[�x�q�[�v
	int dHeap;
	
	// �V�F�[�_�[�q�[�v
	int sHeap;

	// ���\�[�X
	int rsc;
};
