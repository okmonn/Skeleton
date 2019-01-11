#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandQueue;

class Queue
{
public:
	// コンストラクタ
	Queue(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~Queue();

	// キューの取得
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// キューの生成
	long Create(const D3D12_COMMAND_LIST_TYPE& type);


	// キュー
	ID3D12CommandQueue* queue;
};
