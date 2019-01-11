#pragma once
#include <memory>


class Swap;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

private:
	// レンダーの生成
	void Create(void);


	// スワップ
	std::weak_ptr<Swap>swap;

	// ヒープID
	int heap;
};
