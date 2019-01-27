#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class List;
class Swap;
class Depth;

class Render
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

	// �N���A
	void Clear(std::weak_ptr<List>list, std::weak_ptr<Depth>depth);

	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const;

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return rHeap;
	}

private:
	// �����_�[�p�q�[�v�̐���
	long CreateRHeap(void);

	// �����_�[�p���\�[�X�̐���
	long CreateRRsc(void);

	// RTV�̐���
	void RTV(void);

	// ������
	void Init(void);


	// �X���b�v
	std::weak_ptr<Swap>swap;

	// �q�[�v
	ID3D12DescriptorHeap* rHeap;

	// ���\�[�X
	std::vector<ID3D12Resource*>rRsc;
};
