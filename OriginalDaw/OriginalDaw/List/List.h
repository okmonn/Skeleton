#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

class List
{
public:
	// �R���X�g���N�^
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~List();

	// ���Z�b�g
	void Reset(ID3D12PipelineState* pipe = nullptr);

	// �`��p���[�g�V�O�l�`���̃Z�b�g
	void SetRoot(ID3D12RootSignature* root);

	// �R���s���[�g�p���[�g�V�O�l�`���̃Z�b�g
	void SetComputeRoot(ID3D12RootSignature* root);

	// �p�C�v���C���̃Z�b�g
	void SetPipe(ID3D12PipelineState* pipe);

	// �r���[�|�[�g�̃Z�b�g
	void SetView(const unsigned int& width, const unsigned int& height);

	// �V�U�[�̃Z�b�g
	void SetScissor(const unsigned int& width, const unsigned int& height);

	// �o���A
	void Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc);

	// ���X�g�̃N���[�Y
	void Close(void);

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
