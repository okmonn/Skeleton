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

// ���\�[�X��
#define RSC_MAX 5

// �R���X�g���N�^
Fourier::Fourier(const std::wstring & fileName) : 
	type(FourierType::DFT)
{
	Load("fourier", fileName);
}

// �f�X�g���N�^
Fourier::~Fourier()
{
	End();
}

// ������
void Fourier::Init(const size_t & num)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CBV("type", (sizeof(type) + 0xff) &~0xff);
	UAV("input", sizeof(float), num);
	UAV("real",  sizeof(float), num);
	UAV("imag",  sizeof(float), num);
	UAV("index", sizeof(float), num);
}

// �f�[�^�̃R�s�[
void Fourier::Copy(const std::string & name, const std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, data.data(), sizeof(float) * data.size());
}

// �f�[�^�̃R�s�[
void Fourier::Copy(const std::string & name, const FourierType & type)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, &type, sizeof(type));
}

// �f�[�^�̍X�V
void Fourier::UpData(const std::string & name, std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	auto size = DescriptorMane::Get().GetRsc(info[name].rsc)->GetDesc().Width / sizeof(float);

	data.assign((float*)info[name].data, (float*)info[name].data + size);
}

// ���s
void Fourier::Execution(std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag)
{
	Copy("type", type);
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
	UpData("input", input);
	UpData("real", real);
	UpData("imag", imag);

	/*for (unsigned int i = 0; i < input.size(); ++i)
	{
		if (index[i] > i)
		{
			auto re = real[static_cast<unsigned int>(index[i])];
			auto im = imag[static_cast<unsigned int>(index[i])];
			real[static_cast<unsigned int>(index[i])] = real[i];
			imag[static_cast<unsigned int>(index[i])] = imag[i];
			real[i] = re;
			imag[i] = im;
		}
	}*/
}
