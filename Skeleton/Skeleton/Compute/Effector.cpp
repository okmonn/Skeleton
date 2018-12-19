#include "Effector.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// リソースの最大数
#define RSC_MAX 3

// コンストラクタ
Effector::Effector(std::weak_ptr<Device>dev, const std::tstring& fileName)
{
	this->dev = dev;

	param = { 44100.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	
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

	CBV("b0", sizeof(Param));
	UAV("u0", sizeof(float), num);
	UAV("u1", sizeof(float), num);
}

// 実行
void Effector::Execution(const std::vector<float>& input, std::vector<float>& out)
{
	memcpy(info["b0"].data, &param, sizeof(Param));

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

// サンプリング周波数のセット
void Effector::SetSample(const float & sample)
{
	if (sample < 0.0f)
	{
		return;
	}

	param.sample = sample;
}

// 増幅率のセット
void Effector::SetGain(const float & gain)
{
	if (gain < 0.0f)
	{
		return;
	}

	param.gain = gain;
}

// ボリュームのセット
void Effector::SetVolume(const float & volume)
{
	if (volume < 0.0f)
	{
		return;
	}

	param.volume = volume;
}

// 変調深度のセット
void Effector::SetDepth(const float & depth)
{
	param.depth = depth;
}

// 変調周波数のセット
void Effector::SetRate(const float & rate)
{
	param.rate = rate;
}
