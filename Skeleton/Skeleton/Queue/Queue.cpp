#include "Queue.h"
#include "../Release.h"

// コンストラクタ
Queue::Queue(const D3D12_COMMAND_LIST_TYPE & type) : 
	queue(nullptr)
{
	CreateQueue(type);
}

// デストラクタ
Queue::~Queue()
{
	Release(queue);
}

// キューの生成
long Queue::CreateQueue(const D3D12_COMMAND_LIST_TYPE & type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = Dev->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nキューの生成：失敗\n"));
	}

	return hr;
}

// 実行
void Queue::Execution(ID3D12CommandList ** list, const size_t & num) const
{
	queue->ExecuteCommandLists(static_cast<unsigned int>(num), list);
}
