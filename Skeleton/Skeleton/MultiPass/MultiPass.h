#pragma once
#include <string>
#include <memory>

class TextureLoader;
class DescriptorMane;
class Window;
class Device;
class List;
class Queue;
class Depth;
class Fence;
class ShadowMap;
class Root;
class Pipe;

class MultiPass
{
public:
	// �R���X�g���N�^
	MultiPass(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~MultiPass();

	// �`�揀��
	void Set(std::weak_ptr<Depth>depth);

	// ���s
	void Execution(void);

	// �`��
	void Draw(std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<ShadowMap>shadow);

private:
	// ���\�[�X�̐���
	long CreateRsc(void);

	// �����_�[�^�[�Q�b�g�r���[�̐���
	void CreateRenderView(void);

	// �V�F�[�_�[�r���[�̐���
	void CreateShaderView(void);

	// ���_���\�[�X�̐���	
	long CreateVertexRsc(void);

	// ���_�}�b�v
	long Map(void);

	// ������
	void Init(void);

	// �����_�[�^�[�Q�b�g�̃N���A
	void Clear(std::weak_ptr<Depth> depth);


	// �e�N�X�`�����[�_�[
	TextureLoader& tex;

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

	// �����_�[�q�[�v
	int rHeap;

	// �V�F�[�_�[�q�[�v
	int sHeap;

	// ���\�[�X
	int rsc;

	// ���_���\�[�X
	int vRsc;
};
