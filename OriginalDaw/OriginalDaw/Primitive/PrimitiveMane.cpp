#include "PrimitiveMane.h"
#include "Primitive.h"
#include "../Pipe/PipeMane.h"
#include "../etc/Release.h"

// トポロジータイプ
const D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType[] = {
	D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
};

// タイプ
const std::string pipeName[] = {
	"",
	"point",
	"line",
	"triangle"
};

// コンストラクタ
PrimitiveMane::PrimitiveMane(std::weak_ptr<Window> win, std::weak_ptr<Root> root) : win(win), root(root)
{
	primitive.clear();

	CreatePipe();
}

// デストラクタ
PrimitiveMane::~PrimitiveMane()
{
	for (auto& i : pipeName)
	{
		PipeMane::Get().Delete(i);
	}
}

// パイプの生成
void PrimitiveMane::CreatePipe(void)
{
	PipeMane::Get().Get().CreatePipe(pipeName[static_cast<int>(prm::PrimitiveType::point)], root, topologyType[static_cast<int>(prm::PrimitiveType::point)], { 0, 3 }, false);
	PipeMane::Get().Get().CreatePipe(pipeName[static_cast<int>(prm::PrimitiveType::line)], root, topologyType[static_cast<int>(prm::PrimitiveType::line)], { 0, 3 }, false);
	PipeMane::Get().Get().CreatePipe(pipeName[static_cast<int>(prm::PrimitiveType::triangle)], root, topologyType[static_cast<int>(prm::PrimitiveType::triangle)], { 0, 3 }, false);
}

// 描画
void PrimitiveMane::Draw(std::weak_ptr<List> list, const prm::PrimitiveType & type, prm::Vertex * vertex, const size_t & num)
{
	primitive.push_back(std::make_unique<Primitive>(win, root, PipeMane::Get().GetPipe(pipeName[static_cast<int>(type)]), type));
	primitive.back()->Draw(list, vertex, num);
}

// クリア
void PrimitiveMane::Clear(void)
{
	primitive.clear();
}
