#pragma once
#include <DirectXMath.h>

namespace prm {
	// 座標
	struct Vertex
	{
		//座標
		DirectX::XMFLOAT3 pos;
		//色
		DirectX::XMFLOAT4 color;
	};

	// タイプ
	enum class PrimitiveType
	{
		point, 
		line,
		triangle
	};
}
