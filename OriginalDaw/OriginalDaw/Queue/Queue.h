#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandQueue;
struct ID3D12CommandList;

class Queue
{
public:
	// コンストラクタ
	Queue(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~Queue();

	// 実行
	void Execution(ID3D12CommandList** list, const size_t& num);

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
