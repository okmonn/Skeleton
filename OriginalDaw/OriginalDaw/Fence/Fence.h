#pragma once
#include <memory>

struct ID3D12Fence;
class Queue;

class Fence
{
public:
	// コンストラクタ
	Fence(std::weak_ptr<Queue>queue);
	// デストラクタ
	~Fence();

	// 待機
	void Wait(void);

	// フェンスの取得
	ID3D12Fence* Get(void) const {
		return fence;
	}

private:
	// フェンスの生成
	long Create(void);


	// キュー
	std::weak_ptr<Queue>queue;

	// フェンス
	ID3D12Fence* fence;

	// イベント
	void* handle;

	// フェンスカウン
	long long cnt;
};
