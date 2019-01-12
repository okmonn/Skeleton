#pragma once
#include "Primitive.h"

class Point :
	public Primitive
{
public:
	// コンストラクタ
	Point(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Point();

	// 頂点のセット
	void SetVertex(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color);

private:

};
