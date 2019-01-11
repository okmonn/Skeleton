#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;

class List
{
public:
	// �R���X�g���N�^
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~List();

	// �r���[�|�[�g�̃Z�b�g
	void SetView(const unsigned int& width, const unsigned int& height);

	// �V�U�[�̃Z�b�g
	void SetScissor(const unsigned int& width, const unsigned int& height);

	// �A���P�[�^�̎擾
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// ���X�g�̎擾
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// �A���P�[�^�̐���
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// ���X�g�̐���
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);



	// �A���P�[�^
	ID3D12CommandAllocator* allo;

	// ���X�g
	ID3D12GraphicsCommandList* list;
};
