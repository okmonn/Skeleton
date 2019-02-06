#include "List.h"
#include "../Release.h"

// �R���X�g���N�^
List::List(const D3D12_COMMAND_LIST_TYPE & type) : 
	allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �A���P�[�^������
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Dev->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�A���P�[�^�̐����F���s\n"));
	}

	return hr;
}

// ���X�g�̐���
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Dev->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n���X�g�̐����F���s\n"));
		return hr;
	}

	Close();

	return hr;
}

// ���Z�b�g
void List::Reset(ID3D12PipelineState * pipe) const
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// �r���[�|�[�g�̃Z�b�g
void List::Viewport(const Vec2 & size) const
{
	D3D12_VIEWPORT view{};
	view.Height   = static_cast<float>(size.y);
	view.MaxDepth = 1.0f;
	view.Width    = static_cast<float>(size.x);

	list->RSSetViewports(1, &view);
}

// �V�U�[�̃Z�b�g
void List::Scissor(const Vec2 & size) const
{
	RECT scissor{};
	scissor.bottom = static_cast<long>(size.y);
	scissor.right  = static_cast<long>(size.x);

	list->RSSetScissorRects(1, &scissor);
}

// �o���A
void List::Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// �`��p���[�g�̃Z�b�g
void List::Root(ID3D12RootSignature * root) const
{
	list->SetGraphicsRootSignature(root);
}

// �v�Z�p���[�g�̃Z�b�g
void List::ComputeRoot(ID3D12RootSignature * root) const
{
	list->SetComputeRootSignature(root);
}

// �p�C�v���C���̃Z�b�g
void List::Pipe(ID3D12PipelineState * pipe) const
{
	list->SetPipelineState(pipe);
}

// �q�[�v�̃Z�b�g
void List::Heap(ID3D12DescriptorHeap ** heap, const size_t & num) const
{
	list->SetDescriptorHeaps(static_cast<unsigned int>(num), heap);
}

// �q�[�v�ƕ`�惋�[�g�Ƃ̊֘A�t��
void List::RootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index) const
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;

	list->SetGraphicsRootDescriptorTable(id, handle);
}

// �q�[�v�ƌv�Z�p���[�g�̊֘A�t��
void List::ComputeRootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index) const
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;

	list->SetComputeRootDescriptorTable(id, handle);
}

// ���_�o�b�t�@�r���[�̃Z�b�g
void List::VertexView(const D3D12_VERTEX_BUFFER_VIEW & view) const
{
	list->IASetVertexBuffers(0, 1, &view);
}

// �C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g
void List::IndexView(const D3D12_INDEX_BUFFER_VIEW & view) const
{
	list->IASetIndexBuffer(&view);
}

// �g�|���W�[�̃Z�b�g
void List::Topology(const D3D_PRIMITIVE_TOPOLOGY & type) const
{
	list->IASetPrimitiveTopology(type);
}

// ���_�`��
void List::DrawVertex(const size_t & num, const unsigned int & instance) const
{
	list->DrawInstanced(static_cast<unsigned int>(num), instance, 0, 0);
}

// GPU�v�Z�̎��s
void List::Dispatch(const unsigned int & x, const unsigned int & y, const unsigned int & z) const
{
	list->Dispatch(x, y, z);
}

// �o���h���̎��s
void List::Bundle(ID3D12GraphicsCommandList * list) const
{
	this->list->ExecuteBundle(list);
}

// �N���[�Y
void List::Close(void)
{
	list->Close();
}
