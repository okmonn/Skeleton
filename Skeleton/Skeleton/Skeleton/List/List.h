#pragma once
#include "../Vector2.h"

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D_PRIMITIVE_TOPOLOGY : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct D3D12_VERTEX_BUFFER_VIEW;
struct D3D12_INDEX_BUFFER_VIEW;

class List
{
public:
	// �R���X�g���N�^
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~List();

	// ���Z�b�g
	void Reset(ID3D12PipelineState* pipe = nullptr) const;

	// �r���[�|�[�g�̃Z�b�g
	void Viewport(const Vec2& size) const;

	// �V�U�[�̃Z�b�g
	void Scissor(const Vec2& size) const;

	// �o���A�̃Z�b�g
	void Barrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc) const;

	// �`��p���[�g�̃Z�b�g
	void Root(ID3D12RootSignature* root) const;

	// �v�Z�p���[�g�̃Z�b�g
	void  ComputeRoot(ID3D12RootSignature* root) const;

	// �p�C�v�̃Z�b�g
	void Pipe(ID3D12PipelineState* pipe) const;

	// �q�[�v�̃Z�b�g
	void Heap(ID3D12DescriptorHeap** heap, const size_t& num) const;

	// �q�[�v�ƕ`�惋�[�g�Ƃ̊֘A�t��
	void RootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0) const;

	// �q�[�v�ƌv�Z�p���[�g�̊֘A�t��
	void ComputeRootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0) const;

	// ���_�o�b�t�@�r���[�̃Z�b�g
	void VertexView(const D3D12_VERTEX_BUFFER_VIEW& view) const;

	// �C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g
	void IndexView(const D3D12_INDEX_BUFFER_VIEW& view) const;

	// �g�|���W�[�̃Z�b�g
	void Topology(const D3D_PRIMITIVE_TOPOLOGY& type) const;

	// ���_�`��
	void DrawVertex(const size_t& num, const unsigned int& instance = 1) const;

	// GPU�v�Z�̎��s
	void Dispatch(const unsigned int& x, const unsigned int& y, const unsigned int& z) const;

	// �o���h���̎��s
	void Bundle(ID3D12GraphicsCommandList* list) const;

	// �N���[�Y
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
	// �A���P�[�^������
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// ���X�g�̐���
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);


	// �A���P�[�^
	ID3D12CommandAllocator* allo;

	// ���X�g
	ID3D12GraphicsCommandList* list;
};
