#include "Fourier.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include <d3d12.h>

// リソース数
#define RSC_MAX 4

// コンストラクタ
Fourier::Fourier(std::wstring & fileName)
{
	Load("fourier", fileName);
}

// デストラクタ
Fourier::~Fourier()
{
	End();
}

// 初期化
void Fourier::Init(const size_t & num)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	UAV("input", sizeof(float), num);
	UAV("real",  sizeof(float), num);
	UAV("imag",  sizeof(float), num);
	UAV("index", sizeof(float), num);
}

// データのコピー
void Fourier::Copy(const std::string & name, const std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, data.data(), sizeof(float) * data.size());
}

// データの更新
void Fourier::UpData(const std::string & name, std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	data.assign((float*)info[name].data, (float*)info[name].data + data.size());
}

// 実行
void Fourier::Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float> imag)
{
	Copy("input", input);

	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("fourier")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("fourier")->Get());

	SetHeap();
	SetRsc();

	unsigned int num = static_cast<unsigned int>(std::log2(input.size()));
	list->Dispatch(num, 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	std::vector<float>index;
	UpData("index", index);
	UpData("real", real);
	UpData("imag", imag);

	for (unsigned int i = 0; i < input.size(); ++i)
	{
		if (index[i] > i)
		{
			auto re        = real[index[i]];
			auto im        = imag[index[i]];
			real[index[i]] = real[i];
			imag[index[i]] = imag[i];
			real[i]        = re;
			imag[i]        = im;
		}
	}
}
