#include "Point.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// 頂点数
#define VERTEX_MAX 1

// コンストラクタ
Point::Point(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe)
{
	this->win = win;
	this->root = root;
	this->pipe = pipe;

	vertex.resize(VERTEX_MAX);
}

// デストラクタ
Point::~Point()
{
	DescriptorMane::Get().DeleteRsc(constant);
	DescriptorMane::Get().DeleteRsc(vRsc);
	DescriptorMane::Get().DeleteHeap(constant);
}

// 頂点のセット
void Point::SetVertex(const DirectX::XMFLOAT2 & pos, const DirectX::XMFLOAT4 & color)
{
	vertex[0] = { { pos.x, pos.y, 0.0f }, color };

	ConstantBuffer();
	VertexBuffer();
	Bundle(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}
