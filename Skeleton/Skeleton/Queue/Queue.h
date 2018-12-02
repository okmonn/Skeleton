#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandQueue;
class Device;

class Queue
{
public:
	// コンストラクタ
	Queue(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = (D3D12_COMMAND_LIST_TYPE)0);
	// デストラクタ
	~Queue();

	// キューの取得
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// キューの生成
	long CreateQueue(const D3D12_COMMAND_LIST_TYPE& type);


	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	ID3D12CommandQueue* queue;
};
