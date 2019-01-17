#include "Effector.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include <d3d12.h>

#define RSC_MAX 3

// コンストラクタ
Effector::Effector(const std::wstring & fileName)
{
	Load("effect", fileName);

	Init();
}

// デストラクタ
Effector::~Effector()
{
	End();
}

// 初期化
void Effector::Init(void)
{
	DescriptorMane::Get().CreateHeap(heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CBV("param", (sizeof(snd::Param) + 0xff) &~ 0xff);
	UAV("input",  sizeof(float), 4410);
	UAV("output", sizeof(float), 4410);
}

// データのコピー
void Effector::Copy(const std::string & name, const std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, data.data(), sizeof(float) * data.size());
}

// データのコピー
void Effector::Copy(const std::string & name, const snd::Param & param)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, &param, sizeof(snd::Param));
}

// データの更新
void Effector::UpData(const std::string & name, std::vector<float>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	data.assign((float*)info[name].data, (float*)info[name].data + data.size());
}

// 実行
void Effector::Execution(std::vector<float> & data)
{
	Copy("input", data);

	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("effect")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("effect")->Get());

	SetHeap();
	SetRsc();

	list->Dispatch(data.size(), 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	UpData("output", data);
}
