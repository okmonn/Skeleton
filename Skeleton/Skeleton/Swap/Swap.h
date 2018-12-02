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

	// スワップチェインの取得
	IDXGISwapChain4* Get(void) const {
		return swap;
	}

private:
	// スワップチェインの生成
	long CreateSwap(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// キュー
	std::weak_ptr<Queue>queue;

	// スワップチェイン
	IDXGISwapChain4* swap;
};
