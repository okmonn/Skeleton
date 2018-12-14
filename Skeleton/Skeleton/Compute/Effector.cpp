#include "Effector.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../etc/Release.h"

// リソースの最大数
#define RSC_MAX 2

// コンストラクタ
Effector::Effector(std::weak_ptr<Device>dev, const std::tstring& fileName)
{
	this->dev = dev;

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

	UAV("u0", sizeof(float), num);
	UAV("u1", sizeof(float), num);
}

// 実行
void Effector::Execution(const std::vector<float>& input, std::vector<float>& out)
{
	{
		out.resize(input.size(), 0.0f);
		auto hanning = [&](const int& index, const int& size)->float {
			float tmp = 0.0f;

			tmp = (size % 2 == 0) ?
				//偶数
				0.5f - 0.5f * cosf(2.0f * 3.14159265f * (float)index / (float)size) :
				//奇数
				0.5f - 0.5f * cosf(2.0f * 3.14159265f * ((float)index + 0.5f) / (float)size);

			if (tmp == 0.0f)
			{
				tmp = 1.0f;
			}

			return tmp;
		};
		auto sinc = [&](const float& x)->float {
			return (x == 0.0f) ? 1.0f : sinf(x) / x;
		};

		float fe = 1000.0f / 44100.0f; /* エッジ周波数 */
		float delta = 1000.0f / 44100.0f; /* 遷移帯域幅 */

		int J = (int)(3.1 / delta + 0.5) - 1; /* 遅延器の数 */
		if (J % 2 == 1)
		{
			J++; /* J+1が奇数になるように調整する */
		}
		std::vector<float>b(J + 1, 0.0f);

		int m;
		int offset;

		offset = J / 2;
		for (m = -J / 2; m <= J / 2; m++)
		{
			b[offset + m] = 2.0 * fe * sinc(2.0 * 3.14159265f * fe * m);
		}

		for (m = 0; m < J + 1; m++)
		{
			b[m] *= hanning(m, J + 1);
		}

		for (int n = 0; n < input.size(); n++)
		{
			for (m = 0; m <= J; m++)
			{
				if (n - m >= 0)
				{
					out[n] += b[m] * input[n - m];
				}
			}
		}
	}

	return;

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

	handle.ptr = h->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u0"].rsc;
	list->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr = h->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u1"].rsc;
	list->GetList()->SetComputeRootDescriptorTable(1, handle);

	list->GetList()->Dispatch(static_cast<unsigned int>(input.size()), 1, 1);

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};

	queue->Get()->ExecuteCommandLists(1, com);

	fence->Wait();

	out.assign((float*)info["u1"].data, (float*)info["u1"].data + input.size());
}
