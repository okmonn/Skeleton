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
#define RSC_MAX 5

// コンストラクタ
Fourier::Fourier(const std::wstring & fileName)
{
	param = {};

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

	CBV("param", (sizeof(param) + 0xff) &~0xff);
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

// データのコピー
void Fourier::Copy(const std::string & name, const Param & param)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, &param, sizeof(param));
}

// データの更新
void Fourier::UpData(const std::string & name, std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	auto size = DescriptorMane::Get().GetRsc(info[name].rsc)->GetDesc().Width / sizeof(float);

	data.assign((float*)info[name].data, (float*)info[name].data + size);
}

// 実行
void Fourier::Execution(std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	Copy("param", param);
	Copy("input", input);

	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("fourier")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("fourier")->Get());

	SetHeap();
	SetRsc();

	unsigned int num = (param.type == FourierType::FFT || param.type == FourierType::IFFT) ? param.stage : input.size();

	list->Dispatch(num, 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	std::vector<float>index;
	UpData("index", index);
	UpData("input", input);
	UpData("real", real);
	UpData("imag", imag);

	if (!(param.type == FourierType::DFT || param.type == FourierType::IDFT))
	{
		for (unsigned int i = 0; i < index.size(); ++i)
		{
			if (index[i] > i)
			{
				float re = real[index[i]];
				float im = imag[index[i]];
				real[index[i]] = real[i];
				imag[index[i]] = imag[i];
				real[i] = re;
				imag[i] = im;
			}
			if (param.type == FourierType::IFFT)
			{
				real[i] /= real.size();
				imag[i] /= imag.size();
			}
		}
		input.assign(&real[0], &real[input.size()]);
	}
}
