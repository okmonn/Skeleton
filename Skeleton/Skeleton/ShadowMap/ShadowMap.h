#pragma once
#include <memory>

class DescriptorMane;
class Window;
class Device;
class List;
class Queue;
class Fence;

class ShadowMap
{
public:
	// �R���X�g���N�^�@
	ShadowMap(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~ShadowMap();

	// �`�揀��
	void Set(void);

	// ���s
	void Execution(void);

	// �[�x�q�[�vID�̎擾
	int& GetDepthHeap(void) {
		return dHeap;
	}
	// �V�F�[�_�[�q�[�vID�̎擾
	int& GetShaderHeap(void) {
		return sHeap;
	}

private:
	// ���\�[�X�̐���
	long CreateRsc(void);

	// �[�x�r���[�̐���
	void CreateDepthView(void);

	// �V�F�[�_�[�r���[�̐���
	void CreateShaderView(void);

	// ������
	void Init(void);


	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::weak_ptr<List>list;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �[�x�q�[�v
	int dHeap;
	
	// �V�F�[�_�[�q�[�v
	int sHeap;

	// ���\�[�X
	int rsc;
};
