#include "Dft.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/RootMane.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"
#include "../Pipe/Pipe.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include <d3d12.h>

// ���\�[�X��
#define RSC_MAX 3

// �R���X�g���N�^
DFT::DFT(const size_t & num)
{
	Load("dft", L"Shader/DFT.hlsl");

	Init(num);
}

// �f�X�g���N�^
DFT::~DFT()
{
	End();
}

// ������
void DFT::Init(const size_t & num)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	UAV("input", sizeof(float), num);
	UAV("real",  sizeof(float), num);
	UAV("imag",  sizeof(float), num);
}

// ���s
void DFT::Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	Copy("input", input);
	Reset("real");
	Reset("imag");

	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("dft")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("dft")->Get());

	SetHeap();
	SetRsc();

	list->Dispatch(input.size(), 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	UpData("real", real);
	UpData("imag", imag);
}
