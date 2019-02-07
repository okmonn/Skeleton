#pragma once
#include <memory>

struct IDXGISwapChain4;
class Window;
class Queue;

class Swap
{
public:
	// コンストラクタ
	Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Swap();

	// 裏・表画面の遷移
	void Present(void) const;

	// スワップの取得
	IDXGISwapChain4* Get(void) const {
		return swap;
	}

private:
	// スワップの生成
	long CreateSwap(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// キュー
	std::weak_ptr<Queue>queue;

	// スワップ
	IDXGISwapChain4* swap;
};
