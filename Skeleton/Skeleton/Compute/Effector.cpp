#include "Effector.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// リソースの最大数
#define RSC_MAX 4

// コンストラクタ
Effector::Effector(std::weak_ptr<Device>dev, const std::tstring& fileName)
{
	this->dev = dev;

	param = {};
	coe.resize(256);

	Create();
	CreateRoot(fileName);
	CreatePipe();
}

// デストラクタ
Effector::~Effector()
{
	End();

	Release(pipe);
	Release(root);
	Release(shader);
}

// 初期化
void Effector::Init(const unsigned int & num)
{
	descMane.CreateHeap(dev, heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_MAX);

	CBV("b0", sizeof(float) * coe.size());
	UAV("u0", sizeof(float), num);
	UAV("u1", sizeof(float), num);

	LowPass(1000.0f);
}

// ローパスフィルタ
void Effector::LowPass(const float & cutoff)
{
	float fe = 500.0f / 44100.0f;
	float delta = 1000.0f / 44100.0f;
	int num = (int)(3.1f / delta + 0.5f) - 1;
	if (num % 2 != 0)
	{
		++num;
	}
	int offset = num / 2;
	for (int i = -offset; i <= offset; ++i)
	{
		coe[offset + i] = (2.0f * fe * Sinc(2.0f * 3.14159265f * fe * i)) * Haninng(offset + i, num + 1);
	}
	memcpy(info["b0"].data, coe.data(), sizeof(float) * coe.size());
}

// パラメータのセット
void Effector::SetParam(void)
{
	param.sample = 44100.0f;
	//遷移帯域幅
	float delta = 1000.0f / 44100.0f;
	param.delayDevNum = std::roundf(3.1f / delta) - 1.0f;
	if ((int)param.delayDevNum % 2 == 1)
	{
		++param.delayDevNum;
	}
}

// 実行
void Effector::Execution(const std::vector<float>& input, std::vector<float>& out)
{
	//データの更新
	memcpy(info["u0"].data, &input[0], sizeof(float) * input.size());
	memset(info["u1"].data, 0, sizeof(float) * input.size());

	list->Reset();

	list->GetList()->SetComputeRootSignature(root);
	list->GetList()->SetPipelineState(pipe);

	auto h      = descMane.GetHeap(heap);
	auto handle = h->GetGPUDescriptorHandleForHeapStart();
	auto size   = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	list->GetList()->SetDescriptorHeaps(1, &h);

	handle.ptr = h->GetGPUDescriptorHandleForHeapStart().ptr + size * info["b0"].rsc;
	list->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr = h->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u0"].rsc;
	list->GetList()->SetComputeRootDescriptorTable(1, handle);
	handle.ptr = h->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u1"].rsc;
	list->GetList()->SetComputeRootDescriptorTable(2, handle);

	list->GetList()->Dispatch(static_cast<unsigned int>(input.size()), 1, 1);

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};

	queue->Get()->ExecuteCommandLists(1, com);

	fence->Wait();

	out.assign((float*)info["u1"].data, (float*)info["u1"].data + input.size());
}

// ハニング窓関数
float Effector::Haninng(const int & i, const int & size)
{
	float tmp = 0.0f;

	tmp = (size % 2 == 0) ?
		//偶数
		0.5f - 0.5f * cos(2.0f * 3.14159265f * (float)i / (float)size) :
		//奇数
		0.5f - 0.5f * cos(2.0f * 3.14159265f * ((float)i + 0.5f) / (float)size);

	if (tmp == 0.0f)
	{
		tmp = 1.0f;
	}

	return tmp;
}

// シンク関数
float Effector::Sinc(const float & i)
{
	return (i == 0.0f) ? 1.0f : sinf(i) / i;
}
