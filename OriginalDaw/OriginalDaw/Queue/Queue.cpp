#include "Queue.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

// コンストラクタ
Queue::Queue(const D3D12_COMMAND_LIST_TYPE & type) :
	queue(nullptr)
{
	Create(type);
}

// デストラクタ
Queue::~Queue()
{
	Release(queue);
}

// キューの生成
long Queue::Create(const D3D12_COMMAND_LIST_TYPE & type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = Device::Get().GetDev()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nキューの生成：失敗\n"));
	}

	return hr;
}
