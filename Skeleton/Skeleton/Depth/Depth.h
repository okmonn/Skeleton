#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Window;
class List;

class Depth
{
public:
	// �R���X�g���N�^
	Depth(std::weak_ptr<Window>win);
	// �f�X�g���N�^
	~Depth();

	// �N���A
	void Clear(std::weak_ptr<List>list);

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return dHeap;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const {
		return dRsc;
	}

private:
	// �f�v�X�p�q�[�v�̐���
	long CreateDHeap(void);

	// �f�v�X�p���\�[�X�̐���
	long CreateDRsc(void);

	// DSV�̐���
	void DSV(void);

	// ������
	void Init(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�v�X�p�q�[�v
	ID3D12DescriptorHeap* dHeap;

	// �f�v�X�p���\�[�X
	ID3D12Resource* dRsc;
};
