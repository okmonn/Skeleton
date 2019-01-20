#include "Fft.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/RootMane.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"
#include "../Pipe/Pipe.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include <d3d12.h>

// リソース数
#define RSC_MAX 4

// コンストラクタ
FFT::FFT(const size_t & num)
{
	Load("fft", L"Shader/FFT.hlsl");
	Init(num);
}

// デストラクタ
FFT::~FFT()
{
	End();
}

// 初期化
void FFT::Init(const size_t & num)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	unsigned int stage = static_cast<unsigned>(std::ceil(log2(num)));
	unsigned int size = static_cast<unsigned int>(std::pow(2, stage));

	UAV("input", sizeof(float), size);
	UAV("real",  sizeof(float), size);
	UAV("imag",  sizeof(float), size);
	UAV("index", sizeof(float), size);
}

// 実行
void FFT::Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	Copy("input", input);
	Copy("real",  real);
	Copy("imag",  imag);

	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("fft")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("fft")->Get());

	SetHeap();
	SetRsc();

	unsigned int stage = static_cast<unsigned int>(std::ceil(std::log2(input.size())));

	list->Dispatch(stage, 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	std::vector<float>index;
	UpData("real", real);
	UpData("imag", imag);
	UpData("index", index);

	for (unsigned int i = 0; i < index.size(); ++i)
	{
		if (static_cast<unsigned int>(index[i]) > i)
		{
			float re = real[static_cast<unsigned int>(index[i])];
			float im = imag[static_cast<unsigned int>(index[i])];
			real[static_cast<unsigned int>(index[i])] = real[i];
			imag[static_cast<unsigned int>(index[i])] = imag[i];
			real[i] = re;
			imag[i] = im;
		}
	}
}
