#include "Effector.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Release.h"

// リソース数
#define RSC_NUM 3

// コンストラクタ
Effector::Effector(const std::string & fileName)
{
	param = { 1.0f, 1.0f, 1.0f, 1.0f, (1.0f / 10.0f), 0.0f };

	Load("effector", fileName);
}

// デストラクタ
Effector::~Effector()
{
	End();
}

// 初期化
void Effector::Init(const size_t & num)
{
	CreateHeap(RSC_NUM);

	CBV("param",  sizeof(snd::Param));
	UAV("input",  sizeof(float), num);
	UAV("output", sizeof(float), num);
}

// コピー
void Effector::Copy(const std::string & name, const snd::Param & param)
{
	if (rsc.find(name) == rsc.end())
	{
		return;
	}

	memcpy(this->data[name], &param, sizeof(param));
}

// コピー
void Effector::Copy(const std::string & name, const std::vector<float>& data)
{
	if (rsc.find(name) == rsc.end())
	{
		return;
	}

	memcpy(this->data[name], data.data(), sizeof(float) * data.size());
}

// 更新
void Effector::UpData(const std::string & name, std::vector<float>& data)
{
	if (this->data.find(name) == this->data.end())
	{
		return;
	}

	unsigned int size = static_cast<unsigned int>(rsc[name]->GetDesc().Width) / sizeof(float);

	data.assign(reinterpret_cast<float*>(this->data[name]), reinterpret_cast<float*>(this->data[name]) + size);
}

// 実行
std::vector<float> Effector::Execution(const std::vector<float> & input)
{
	Copy("param", param);
	Copy("input", input);

	list->Reset();

	list->ComputeRoot(RootMane::Get().GetRoot("effector")->Get());
	list->Pipe(PipeMane::Get().GetPipe("effector")->Get());

	list->Heap(&heap, 1);

	for (unsigned int i = 0; i < RSC_NUM; ++i)
	{
		list->ComputeRootTable(i, heap, i);
	}

	list->Dispatch(input.size(), 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList()
	};
	queue->Execution(com, _countof(com));

	fence->Wait();

	std::vector<float>tmp;
	UpData("output", tmp);

	return tmp;
}
