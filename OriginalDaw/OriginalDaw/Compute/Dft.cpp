#include "Dft.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/RootMane.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"
#include "../Pipe/Pipe.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 3

// コンストラクタ
DFT::DFT(const size_t & num)
{
	Load("dft", L"Shader/DFT.hlsl");

	Init(num);
}

// デストラクタ
DFT::~DFT()
{
	End();
}

// 初期化
void DFT::Init(const size_t & num)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	UAV("input", sizeof(float), num);
	UAV("real",  sizeof(float), num);
	UAV("imag",  sizeof(float), num);
}

// 実行
void DFT::Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	if (input.size() > D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION)
	{
		OutputDebugString(_T("\n最大スレッド数を超えています\n"));
		return;
	}

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

// 実行
void DFT::Execution(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag)
{
	if (input.size() > D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION)
	{
		OutputDebugString(_T("\n最大スレッド数を超えています\n"));
		return;
	}

	std::vector<float>i((float*)&input[0], (float*)&input[input.size() - 1]);
	std::vector<float>re;
	std::vector<float>im;

	Copy("input", i);
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

	UpData("real", re);
	UpData("imag", im);

	real.assign((double*)&re[0], (double *)&re[re.size() - 1]);
	imag.assign((double*)&im[0], (double *)&im[im.size() - 1]);
}
